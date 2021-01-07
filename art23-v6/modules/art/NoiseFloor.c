

#include <stdio.h>
#include <stdlib.h>


#include "smatch.h"
#include "TimeMillisecond.h"
#include "ParameterSelect.h"
#include "Card.h"
#include "Device.h"
#include "MyDelay.h"
#include "Field.h"
#include "NewArt.h"
#include "CommandParse.h"
#include "ParameterParse.h"		

#include "NoiseFloor.h"

#include "wlantype.h"
#include "rate_constants.h"
#include "LinkList.h"

#include "UserPrint.h"
#include "ErrorPrint.h"
#include "ParseError.h"
#include "NartError.h"
#include "CardError.h"

#define MBUFFER 1024

#define MATTEMPT 11
#define MERROR 5
#define MCHAIN 3

static void Sort(int *xp, int *sort, int np)
{
	int it, ip, jp;
	int min;

	for(it=0; it<np; it++)
	{
	    min= -1;
	    for(ip=0; ip<np; ip++)
		{
	    	//
		    // see if this one has already been used
		    //
		    for(jp=0; jp<it; jp++)
			{
			    if(ip==sort[jp])
				{
			    	break;
				}
			}
		    //
		    // got a new one to check
		    //
		    if(jp>=it)
			{
			    //
			    // is it the new minimum?
			    //
		        if(min<0 || xp[ip]<xp[min])
				{
			        //
			        // found new minimum
			        //
			        min=ip;
				}
			}
		}
	    sort[it]=min;
	}
}


int NoiseFloorFetch(int *nfc, int *nfe, int nfn)
{
	unsigned int mask;
	int it;
	unsigned int address;
	int high, low;
	//
	// noise floor values are signed. this computes a mask to extend the sign bit
	// it assumes that all the values are the same length.
	//
	mask=0;
	if(FieldFind("BB_cca_b0.minCCApwr_0", &address, &low, &high))
	{
		for(it=high-low+1; it<32; it++)
		{
			mask |= (1<<it);
		}
	}
	//
	// fetch the answers
	//
	FieldRead("BB_cca_b0.minCCApwr_0", (unsigned int *)&nfc[0]);
	nfc[0]|=mask;
	FieldRead("BB_cca_b1.minCCApwr_1", (unsigned int *)&nfc[1]);   
	nfc[1]|=mask;
	FieldRead("BB_cca_b2.minCCApwr_2", (unsigned int *)&nfc[2]);
	nfc[2]|=mask;
	FieldRead("BB_ext_chan_pwr_thr_2_b0.minCCApwr_ext_0", (unsigned int *)&nfe[0]);
	nfe[0]|=mask;
	FieldRead("BB_ext_chan_pwr_thr_2_b1.minCCApwr_ext_1", (unsigned int *)&nfe[1]);
	nfe[1]|=mask;
	FieldRead("BB_ext_chan_pwr_thr_2_b2.minCCApwr_ext_2", (unsigned int *)&nfe[2]);
	nfe[2]|=mask;

	return 0;
}


int NoiseFloorFetchWait(int *nfc, int *nfe, int nfn, int timeout)
{
	unsigned int mask;
	int it;
	unsigned int address;
	int high, low;
	int ip;
	unsigned int ready;
	//
	// noise floor values are signed. this computes a mask to extend the sign bit
	// it assumes that all the values are the same length.
	//
	mask=0;
	if(FieldFind("BB_cca_b0.minCCApwr_0", &address, &low, &high))
	{
		for(it=high-low+1; it<32; it++)
		{
			mask |= (1<<it);
		}
	}
	for(ip=0; ip<timeout; ip++)
	{
		//
		// wait for noise floor calibration to finish
		//
		FieldRead("BB_agc_control.do_noisefloor",&ready);
		if(ready==0)
		{
			NoiseFloorFetch(nfc, nfe, nfn);
			break;
		}
		else
		{
			UserPrint(".");
			MyDelay(1);
		}
	}
	if(ip>=timeout)
	{
		UserPrint("timeout.");
		nfc[0]=0;
		nfc[1]=0;
		nfc[2]=0;
		nfe[0]=0;
		nfe[1]=0;
		nfe[2]=0;
		return -1;
	}
	return 0;
}


int NoiseFloorLoad(int *nfc, int *nfe, int nfn)
{
    FieldWrite("BB_cca_b0.cf_maxCCApwr_0",2*nfc[0]);
    FieldWrite("BB_cca_b1.cf_maxCCApwr_1",2*nfc[1]);
    FieldWrite("BB_cca_b2.cf_maxCCApwr_2",2*nfc[2]);
    FieldWrite("BB_ext_chan_pwr_thr_2_b0.cf_maxCCApwr_ext_0",2*nfe[0]);
    FieldWrite("BB_ext_chan_pwr_thr_2_b1.cf_maxCCApwr_ext_1",2*nfe[1]);
    FieldWrite("BB_ext_chan_pwr_thr_2_b2.cf_maxCCApwr_ext_2",2*nfe[2]);

	FieldWrite("BB_agc_control.enable_noisefloor",0);
	FieldWrite("BB_agc_control.no_update_noisefloor",0);
	FieldWrite("BB_agc_control.do_noisefloor",1);

	return 0;
}


int NoiseFloorLoadWait(int *nfc, int *nfe, int nfn, int timeout)
{
	int ip;
	unsigned int ready;

	NoiseFloorLoad(nfc,nfe,nfn);

	for(ip=0; ip<timeout; ip++)
	{
		//
		// wait for noise floor calibration to finish
		//
		FieldRead("BB_agc_control.do_noisefloor",&ready);
		if(ready==0)
		{
			break;
		}
		UserPrint(".");
	}
	if(ip>=timeout)
	{
		UserPrint("timeout.");
		return -1;
	}
	return 0;
}


int NoiseFloorDo(int frequency, int *tnf, int tnfmany, int margin, int attempt, int timeout, int (*done)(),
				 int *nfc, int *nfe, int nfmax) 
{
	int nf[2*MCHAIN][MATTEMPT],diff[2*MCHAIN],nfuse[2*MCHAIN],nfverify[2*MCHAIN];
	int sort[MATTEMPT],esort[MATTEMPT];
	int bad;
	int it;
	int ich;
	char buffer[MBUFFER];
	char *cetext[2]={"c","e"};

	SformatOutput(buffer,MBUFFER-1,"|nf|frequency|chain|minimum|maximum|median|used|delta|status|eminimum|emaximum|emedian|eused|edelta|estatus|");
	ErrorPrint(NartDataHeader,buffer);
    //
	// try to calculate the noise floor
	//
    if(attempt>0 && margin>0)
	{
		bad=0;
		if(attempt<0 || attempt>MATTEMPT)
		{
			attempt=MATTEMPT;
		}
		for(it=0; it<attempt; it++)
		{
			UserPrint(" %d",it);

			for(ich=0; ich<2*MCHAIN; ich++)
			{
				nfuse[ich]= -50;
			}
			NoiseFloorLoadWait(nfuse,&nfuse[MCHAIN],MCHAIN,timeout);

			FieldWrite("BB_agc_control.enable_noisefloor",1);
			FieldWrite("BB_agc_control.no_update_noisefloor",1);
			FieldWrite("BB_agc_control.do_noisefloor",1);

			NoiseFloorFetchWait(nfuse,&nfuse[MCHAIN],MCHAIN,timeout);

			for(ich=0; ich<2*MCHAIN; ich++)
			{
				nf[ich][it]=nfuse[ich];
			}
		}
		//
		// compute median
		//
		UserPrint("\n");
		for(ich=0; ich<MCHAIN; ich++)
		{
			UserPrint("%d: ",ich);
			for(it=0; it<attempt; it++)
			{
				UserPrint("%5d",nf[ich][it]);
			}
			UserPrint("\n   ");
			Sort(nf[ich],sort,attempt);
			for(it=0; it<attempt; it++)
			{
				UserPrint("%5d",nf[ich][sort[it]]);
			}
			nfuse[ich]=nf[ich][sort[attempt/2]];
			UserPrint(" -> %5d",nfuse[ich]);
			//
			// compare to target
			//
			diff[ich]=nfuse[ich]-tnf[ich%tnfmany];
			if(diff[ich]>margin)
			{
				UserPrint("  BAD , changing to %d\n",tnf[ich%tnfmany]+margin);
				nfuse[ich]=tnf[ich%tnfmany]+margin;
				bad++;
			}
			else if(diff[ich]< -margin)
			{
				UserPrint("  BAD , changing to %d\n",tnf[ich%tnfmany]-margin);
				nfuse[ich]=tnf[ich%tnfmany]-margin;
				bad++;
			}
			else
			{
				UserPrint("  GOOD \n");
			}

			UserPrint("extension %d: ",ich);
			for(it=0; it<attempt; it++)
			{
				UserPrint("%5d",nf[ich+MCHAIN][it]);
			}
			UserPrint("\n   ");
			Sort(nf[ich+MCHAIN],esort,attempt);
			for(it=0; it<attempt; it++)
			{
				UserPrint("%5d",nf[ich+MCHAIN][esort[it]]);
			}
			nfuse[ich+MCHAIN]=nf[ich+MCHAIN][esort[attempt/2]];
			UserPrint(" -> %5d",nfuse[ich+MCHAIN]);
			//
			// compare to target
			//
			diff[ich+MCHAIN]=nfuse[ich+MCHAIN]-tnf[(ich+MCHAIN)%tnfmany];
			if(diff[ich+MCHAIN]>margin)
			{
				UserPrint("  BAD , changing to %d\n",tnf[(ich+MCHAIN)%tnfmany]+margin);
				nfuse[ich+MCHAIN]=tnf[(ich+MCHAIN)%tnfmany]+margin;
				bad++;
			}
			else if(diff[ich+MCHAIN]< -margin)
			{
				UserPrint("  BAD , changing to %d\n",tnf[(ich+MCHAIN)%tnfmany]-margin);
				nfuse[ich+MCHAIN]=tnf[(ich+MCHAIN)%tnfmany]-margin;
				bad++;
			}
			else
			{
				UserPrint("  GOOD \n");
			}

			SformatOutput(buffer,MBUFFER-1,"|nf|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|",
				frequency,ich%MCHAIN,
				nf[ich][sort[0]],nf[ich][sort[attempt-1]],nf[ich][sort[attempt/2]],nfuse[ich],diff[ich],diff[ich]>= -margin && diff[ich]<=margin,
				nf[ich+MCHAIN][esort[0]],nf[ich+MCHAIN][esort[attempt-1]],nf[ich+MCHAIN][esort[attempt/2]],nfuse[ich+MCHAIN],diff[ich+MCHAIN],diff[ich+MCHAIN]>= -margin && diff[ich+MCHAIN]<=margin);
			ErrorPrint(NartData,buffer);
		}
	}
	//
	// just load the user specified values
	//
	else
	{
		bad=0;
		for(ich=0; ich<MCHAIN; ich++)
		{
			nfuse[ich]=tnf[ich%tnfmany];
			UserPrint("%d: %d",ich,nfuse[ich]);
			SformatOutput(buffer,MBUFFER-1,"|nf|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|",
				frequency,ich%MCHAIN,
				0,0,0,nfuse[ich],0,0,
				0,0,0,nfuse[ich+MCHAIN],0,0);
			ErrorPrint(NartData,buffer);
		}

	}

	NoiseFloorLoadWait(nfuse,&nfuse[MCHAIN],MCHAIN,timeout);

	NoiseFloorFetchWait(nfverify,&nfverify[MCHAIN],MCHAIN,timeout);

	for(ich=0; ich<MCHAIN; ich++)
	{
		if(nfuse[ich]!=nfverify[ich])
		{
			UserPrint("NF load problem, ich=%d, %d != %d\n",ich,nfuse[ich],nfverify[ich]);
		}
	}

	for(it=0; it<nfmax; it++)
	{
		nfc[it]=nfuse[it];
		nfe[it]=nfuse[MCHAIN+it];
	}
	
	return -bad;
}

enum NoiseFloorParameter
{
	NoiseFloorTimeout=5000,
	NoiseFloorValue,
	NoiseFloorMargin,
	NoiseFloorAttempt,
};

#define MCHAIN 3

static int NoiseFloorTimeoutMinimum=0;
static int NoiseFloorTimeoutMaximum=100000;
static int NoiseFloorTimeoutDefault=5000;

static int NoiseFloorValueMinimum= -200;
static int NoiseFloorValueMaximum=0;
static int NoiseFloorValueDefault= -100;

static int NoiseFloorMarginMinimum= -1;
static int NoiseFloorMarginMaximum=MERROR;
static int NoiseFloorMarginDefault=MERROR;

static int NoiseFloorAttemptMinimum= -1;
static int NoiseFloorAttemptMaximum=MATTEMPT;
static int NoiseFloorAttemptDefault=MATTEMPT;


static struct _ParameterList NoiseFloorParameter[]=
{
	LINK_FREQUENCY(1),
	LINK_HT40,
	LINK_CHAIN(1),
	LINK_RX_CHAIN,
	LINK_RESET,
	LINK_BANDWIDTH,
	{NoiseFloorTimeout,{"timeout",0,0},"maximum time to wait for noise floor calibration",'d',"ms",1,1,1,\
		&NoiseFloorTimeoutMinimum,&NoiseFloorTimeoutMaximum,&NoiseFloorTimeoutDefault,0,0},
	{NoiseFloorValue,{"value","nf","target"},"expected noise floor value",'d',"dBm",MCHAIN,2,1,\
		&NoiseFloorValueMinimum,&NoiseFloorValueMaximum,&NoiseFloorValueDefault,0,0},
	{NoiseFloorMargin,{"margin","error",0},"margin for acceptable measurements",'d',"dB",1,1,1,\
		&NoiseFloorMarginMinimum,&NoiseFloorMarginMaximum,&NoiseFloorMarginDefault,0,0},
	{NoiseFloorAttempt,{"attempt","iteration",0},"number of measurement attempts",'d',0,1,1,1,\
		&NoiseFloorAttemptMinimum,&NoiseFloorAttemptMaximum,&NoiseFloorAttemptDefault,0,0},
};


void NoiseFloorParameterSplice(struct _ParameterList *list)
{
    list->nspecial=sizeof(NoiseFloorParameter)/sizeof(NoiseFloorParameter[0]);
    list->special=NoiseFloorParameter;
}

void NoiseFloorCommand(int client)
{
	int nfc[MCHAIN],nfe[MCHAIN];
	int ip, np;
	int error;
	int attempt;
	int margin;
	int index;
	int code;
	int timeout;
	int ngot;
	int nf[2*MCHAIN];
	int nfmany;
	char buffer[MBUFFER];
	char *name;
	int frequency;
	int ht40;
	int rxchain;
	int reset;
	int bandwidth;
	//
	// prepare beginning of error message in case we need to use it
	//
	error=0;
	timeout= 100;
	nfmany=1;
	nf[0]=-110;
	margin=MERROR;
	attempt=MATTEMPT;
	frequency= -1;
	ht40=2;
	bandwidth=BW_AUTOMATIC;
	rxchain=0x7;
	reset=0;
	//
	// parse arguments and do it
	//
	np=CommandParameterMany();
	for(ip=0; ip<np; ip++)
	{
		name=CommandParameterName(ip);
		index=ParameterSelectIndex(name,NoiseFloorParameter,sizeof(NoiseFloorParameter)/sizeof(NoiseFloorParameter[0]));
		if(index>=0)
		{
			code=NoiseFloorParameter[index].code;
			switch(code) 
			{
				case LinkParameterFrequency:
					ngot=ParseIntegerList(ip,name,&frequency,&NoiseFloorParameter[index]);
					if(ngot<=0)
					{
						error++;
					}
					break;
				case LinkParameterHt40:
					ngot=ParseIntegerList(ip,name,&ht40,&NoiseFloorParameter[index]);
					if(ngot<=0)
					{
						error++;
					}
					else
					{
						switch(ht40){
						case 0:
							bandwidth=BW_HT20;
							break;
						case 1:
							bandwidth=BW_HT40_PLUS;
							break;
						case -1:
							bandwidth=BW_HT40_MINUS;
							break;
						case 2:
							bandwidth=BW_AUTOMATIC;
							break;
						default:
							error++;
							break;
						}
					}
					break;
				case LinkParameterChain:
				case LinkParameterRxChain:
					ngot=ParseIntegerList(ip,name,&rxchain,&NoiseFloorParameter[index]);
					if(ngot<=0)
					{
						error++;
					}
					break;
				case LinkParameterReset:
					ngot=ParseIntegerList(ip,name,&reset,&NoiseFloorParameter[index]);
					if(ngot<=0)
					{
						error++;
					}
					break;
				case NoiseFloorValue:
					nfmany=ParseInteger(ip,name,2*MCHAIN,nf);
					if(nfmany<=0)
					{
						error++;
					}
					break;
				case NoiseFloorTimeout:
					ngot=ParseInteger(ip,name,1,&timeout);
					if(ngot<=0)
					{
						error++;
					}
					break;
				case NoiseFloorMargin:
					ngot=ParseInteger(ip,name,1,&margin);
					if(ngot<=0)
					{
						error++;
					}
					break;
				case NoiseFloorAttempt:
					ngot=ParseInteger(ip,name,1,&attempt);
					if(ngot<=0)
					{
						error++;
					}
					break;
				case LinkParameterBandwidth:
					ngot=ParseIntegerList(ip,name,&bandwidth,&NoiseFloorParameter[index]);
					if(ngot<=0)
					{
						error++;
					}
					break;
				default:
					ErrorPrint(ParseBadParameter,name);
					error++;
					break;
			}
		}
		else
		{
			error++;
			ErrorPrint(ParseBadParameter,name);
		}
	}
	if(nfmany<=0)
	{
		SendError(client,"target noise floor value is required.");
		error++;
	}
	if(reset)
	{
		if(frequency<=0)
		{
			SendError(client,"frequency is required.");
			error++;
		}
	}
	if(attempt>0 && (attempt%2)==0)
	{
		attempt++;
	}
	//
	// do it
	//
	if(error==0)
	{
		//
		// if there's no card loaded, return error
		//
		if(CardCheckAndLoad(-1)!=0)
		{
			ErrorPrint(CardNoneLoaded);
			return;
		}

		if(frequency>0)
		{
			error=CardResetIfNeeded(frequency,rxchain,rxchain,reset,bandwidth);
			if(error!=0)
			{
				return;
			}
		}

		error=NoiseFloorDo(frequency,nf,nfmany,margin,attempt,timeout,0,nfc,nfe,MCHAIN);
		if(error!=0)
		{
			SformatOutput(buffer,MBUFFER-1,"Noise Floor Calibration error = %d",error);
			buffer[MBUFFER-1]=0;
			SendError(client,buffer);
		}
	}

	SendDone(client);
}


