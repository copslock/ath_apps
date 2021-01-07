/*
 * Copyright (c) 2008-2010, Atheros Communications Inc.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "opt_ah.h"

#ifdef AH_SUPPORT_AR9300

#include "ah.h"
#include "ah_internal.h"
#include "ah_devid.h"
#ifdef AH_DEBUG
#include "ah_desc.h"                    /* NB: for HAL_PHYERR* */
#endif

#include "ar9300/ar9300.h"
#include "ar9300/ar9300reg.h"
#include "ar9300/ar9300phy.h"


void
ar9300GetHwHangs(struct ath_hal *ah, hal_hw_hangs_t *hangs)
{
    struct ath_hal_9300 *ahp = AH9300(ah);
    *hangs = 0;

    if (ar9300GetCapability(ah, HAL_CAP_BB_RIFS_HANG, 0, AH_NULL) == HAL_OK)
        *hangs |= HAL_RIFS_BB_HANG_WAR;
    if (ar9300GetCapability(ah, HAL_CAP_BB_DFS_HANG, 0, AH_NULL) == HAL_OK)
        *hangs |= HAL_DFS_BB_HANG_WAR;
    if (ar9300GetCapability(ah, HAL_CAP_BB_RX_CLEAR_STUCK_HANG, 0, AH_NULL)
        == HAL_OK)
        *hangs |= HAL_RX_STUCK_LOW_BB_HANG_WAR;
    if (ar9300GetCapability(ah, HAL_CAP_MAC_HANG, 0, AH_NULL) == HAL_OK)
        *hangs |= HAL_MAC_HANG_WAR;
    if (ar9300GetCapability(ah, HAL_CAP_PHYRESTART_CLR_WAR, 0, AH_NULL) == HAL_OK)
        *hangs |= HAL_PHYRESTART_CLR_WAR;

    ahp->ah_hang_wars = *hangs;
}

u_int
ar9300GetEifsDur(struct ath_hal *ah)
{
    u_int clks = OS_REG_READ(ah, AR_D_GBL_IFS_EIFS);
    return clks;
}

HAL_BOOL
ar9300SetEifsDur(struct ath_hal *ah, u_int us)
{

    OS_REG_WRITE(ah, AR_D_GBL_IFS_EIFS, us);
    return AH_TRUE;
}

u_int
ar9300GetEifsMask(struct ath_hal *ah)
{
    u_int clks = OS_REG_READ(ah, AR_PHY_ERR_EIFS_MASK);
    return clks;
}

HAL_BOOL
ar9300SetEifsMask(struct ath_hal *ah, u_int us)
{
    OS_REG_WRITE(ah, AR_PHY_ERR_EIFS_MASK, us);
    return AH_TRUE;
}

static u_int
ar9300MacToUsec(struct ath_hal *ah, u_int clks)
{
    HAL_CHANNEL_INTERNAL *chan = AH_PRIVATE(ah)->ah_curchan;

    if (chan && IS_CHAN_HT40(chan))
        return (ath_hal_mac_usec(ah, clks) / 2);
    else
        return (ath_hal_mac_usec(ah, clks));
}

u_int
ar9300MacToClks(struct ath_hal *ah, u_int usecs)
{
    HAL_CHANNEL_INTERNAL *chan = AH_PRIVATE(ah)->ah_curchan;

    if (chan && IS_CHAN_HT40(chan))
        return (ath_hal_mac_clks(ah, usecs) * 2);
    else
        return (ath_hal_mac_clks(ah, usecs));
}

void
ar9300GetMacAddress(struct ath_hal *ah, u_int8_t *mac)
{
    struct ath_hal_9300 *ahp = AH9300(ah);

    OS_MEMCPY(mac, ahp->ah_macaddr, IEEE80211_ADDR_LEN);
}

HAL_BOOL
ar9300SetMacAddress(struct ath_hal *ah, const u_int8_t *mac)
{
    struct ath_hal_9300 *ahp = AH9300(ah);

    OS_MEMCPY(ahp->ah_macaddr, mac, IEEE80211_ADDR_LEN);
    return AH_TRUE;
}

void
ar9300GetBssIdMask(struct ath_hal *ah, u_int8_t *mask)
{
    struct ath_hal_9300 *ahp = AH9300(ah);

    OS_MEMCPY(mask, ahp->ah_bssidmask, IEEE80211_ADDR_LEN);
}

HAL_BOOL
ar9300SetBssIdMask(struct ath_hal *ah, const u_int8_t *mask)
{
    struct ath_hal_9300 *ahp = AH9300(ah);

    /* save it since it must be rewritten on reset */
    OS_MEMCPY(ahp->ah_bssidmask, mask, IEEE80211_ADDR_LEN);

    OS_REG_WRITE(ah, AR_BSSMSKL, LE_READ_4(ahp->ah_bssidmask));
    OS_REG_WRITE(ah, AR_BSSMSKU, LE_READ_2(ahp->ah_bssidmask + 4));
    return AH_TRUE;
}

/*
 * Attempt to change the cards operating regulatory domain to the given value
 * Returns: A_EINVAL for an unsupported regulatory domain.
 *          A_HARDWARE for an unwritable EEPROM or bad EEPROM version
 */
HAL_BOOL
ar9300SetRegulatoryDomain(struct ath_hal *ah,
        u_int16_t regDomain, HAL_STATUS *status)
{
#ifdef AH_SUPPORT_WRITE_REGDOMAIN
    struct ath_hal_9300 *ahp = AH9300(ah);
#endif
    HAL_STATUS ecode;

#ifdef AH_SUPPORT_WRITE_REGDOMAIN
    if (AH_PRIVATE(ah)->ah_currentRD == regDomain) {
        return AH_TRUE;
    }

#if tbd
    if (ahp->ah_eeprotect & AR_EEPROM_PROTECT_WP_128_191)
#else
    if (0)
#endif
    {
        ecode = HAL_EEWRITE;
        goto bad;
    }

    if (ar9300EepromSetParam(ah, EEP_REG_0, regDomain)) {
        HDPRINTF(ah, HAL_DBG_REGULATORY, "%s: set regulatory domain to %u (0x%x)\n",
                __func__, regDomain, regDomain);
        AH_PRIVATE(ah)->ah_currentRD = regDomain;
        return AH_TRUE;
    }
#else
    if(AH_PRIVATE(ah)->ah_currentRD == 0) {
        AH_PRIVATE(ah)->ah_currentRD = regDomain;
        return AH_TRUE;
    }
#endif
    ecode = HAL_EIO;

#ifdef AH_SUPPORT_WRITE_REGDOMAIN
bad:
#endif
    if (status)
        *status = ecode;
    return AH_FALSE;
}

/*
 * Return the wireless modes (a,b,g,t) supported by hardware.
 *
 * This value is what is actually supported by the hardware
 * and is unaffected by regulatory/country code settings.
 *
 */
u_int
ar9300GetWirelessModes(struct ath_hal *ah)
{
    return AH_PRIVATE(ah)->ah_caps.halWirelessModes;
}

/*
 * Set the interrupt and GPIO values so the ISR can disable RF
 * on a switch signal.  Assumes GPIO port and interrupt polarity
 * are set prior to call.
 */
void
ar9300EnableRfKill(struct ath_hal *ah)
{
    /* TODO - can this really be above the hal on the GPIO interface for
     * TODO - the client only?
     */
    struct ath_hal_9300    *ahp = AH9300(ah);

    /* Connect rfsilent_bb_l to baseband */
    OS_REG_SET_BIT(ah, AR_HOSTIF_REG(ah, AR_GPIO_INPUT_EN_VAL), AR_GPIO_INPUT_EN_VAL_RFSILENT_BB);

    /* Set input mux for rfsilent_bb_l to GPIO #0 */
    OS_REG_CLR_BIT(ah, AR_HOSTIF_REG(ah, AR_GPIO_INPUT_MUX2), AR_GPIO_INPUT_MUX2_RFSILENT);
	OS_REG_SET_BIT(ah, AR_HOSTIF_REG(ah, AR_GPIO_INPUT_MUX2), (ahp->ah_gpioSelect & 0x0f) << 4);

    /* Configure the desired GPIO port for input and enable baseband rf silence */
    ath_hal_gpioCfgInput(ah, ahp->ah_gpioSelect);
    OS_REG_SET_BIT(ah, AR_PHY_TEST, RFSILENT_BB);

    /*
     * If radio disable switch connection to GPIO bit x is enabled
     * program GPIO interrupt.
     * If rfkill bit on eeprom is 1, setupeeprommap routine has already
     * verified that it is a later version of eeprom, it has a place for
     * rfkill bit and it is set to 1, indicating that GPIO bit x hardware
     * connection is present.
     */
     /* RFKill uses polling not interrupt, disable interrupt to avoid Eee PC 2.6.21.4 hang up issue */
    if (ath_hal_hasrfkillInt(ah))
    {
        if (ahp->ah_gpioBit == ar9300GpioGet(ah, ahp->ah_gpioSelect)) {
            /* switch already closed, set to interrupt upon open */
            ar9300GpioSetIntr(ah, ahp->ah_gpioSelect, !ahp->ah_gpioBit);
        } else {
            ar9300GpioSetIntr(ah, ahp->ah_gpioSelect, ahp->ah_gpioBit);
        }
    }
}

/*
 * Change the LED blinking pattern to correspond to the connectivity
 */
void
ar9300SetLedState(struct ath_hal *ah, HAL_LED_STATE state)
{
    static const u_int32_t ledbits[8] = {
        AR_CFG_LED_ASSOC_NONE,     /* HAL_LED_RESET */
        AR_CFG_LED_ASSOC_PENDING,  /* HAL_LED_INIT  */
        AR_CFG_LED_ASSOC_PENDING,  /* HAL_LED_READY */
        AR_CFG_LED_ASSOC_PENDING,  /* HAL_LED_SCAN  */
        AR_CFG_LED_ASSOC_PENDING,  /* HAL_LED_AUTH  */
        AR_CFG_LED_ASSOC_ACTIVE,   /* HAL_LED_ASSOC */
        AR_CFG_LED_ASSOC_ACTIVE,   /* HAL_LED_RUN   */
        AR_CFG_LED_ASSOC_NONE,
    };

    OS_REG_RMW_FIELD(ah, AR_CFG_LED, AR_CFG_LED_ASSOC_CTL, ledbits[state]);
}

/*
 * Sets the Power LED on the cardbus without affecting the Network LED.
 */
void
ar9300SetPowerLedState(struct ath_hal *ah, u_int8_t enabled)
{
    u_int32_t    val;

    val = enabled ? AR_CFG_LED_MODE_POWER_ON : AR_CFG_LED_MODE_POWER_OFF;

    OS_REG_RMW_FIELD(ah, AR_CFG_LED, AR_CFG_LED_POWER, val);
}

/*
 * Sets the Network LED on the cardbus without affecting the Power LED.
 */
void
ar9300SetNetworkLedState(struct ath_hal *ah, u_int8_t enabled)
{
    u_int32_t    val;

    val = enabled ? AR_CFG_LED_MODE_NETWORK_ON : AR_CFG_LED_MODE_NETWORK_OFF;

    OS_REG_RMW_FIELD(ah, AR_CFG_LED, AR_CFG_LED_NETWORK, val);
}

/*
 * Change association related fields programmed into the hardware.
 * Writing a valid BSSID to the hardware effectively enables the hardware
 * to synchronize its TSF to the correct beacons and receive frames coming
 * from that BSSID. It is called by the SME JOIN operation.
 */
void
ar9300WriteAssocid(struct ath_hal *ah, const u_int8_t *bssid, u_int16_t assocId)
{
    struct ath_hal_9300 *ahp = AH9300(ah);

    /* save bssid and assocId for restore on reset */
    OS_MEMCPY(ahp->ah_bssid, bssid, IEEE80211_ADDR_LEN);
    ahp->ah_assocId = assocId;

    OS_REG_WRITE(ah, AR_BSS_ID0, LE_READ_4(ahp->ah_bssid));
    OS_REG_WRITE(ah, AR_BSS_ID1, LE_READ_2(ahp->ah_bssid+4) |
                                 ((assocId & 0x3fff)<<AR_BSS_ID1_AID_S));
}

/*
 * Get the current hardware tsf for stamlme
 */
u_int64_t
ar9300GetTsf64(struct ath_hal *ah)
{
    u_int64_t tsf;

    /* XXX sync multi-word read? */
    tsf = OS_REG_READ(ah, AR_TSF_U32);
    tsf = (tsf << 32) | OS_REG_READ(ah, AR_TSF_L32);
    return tsf;
}

u_int64_t
ar9300GetTsf2_64(struct ath_hal *ah)
{
    u_int64_t tsf;

    /* XXX sync multi-word read? */
    tsf = OS_REG_READ(ah, AR_TSF2_U32);
    tsf = (tsf << 32) | OS_REG_READ(ah, AR_TSF2_L32);
    return tsf;
}

void
ar9300SetTsf64(struct ath_hal *ah, u_int64_t tsf)
{
    OS_REG_WRITE(ah, AR_TSF_L32, (tsf & 0xffffffff));
    OS_REG_WRITE(ah, AR_TSF_U32, ((tsf >> 32) & 0xffffffff));
}

/*
 * Get the current hardware tsf for stamlme
 */
u_int32_t
ar9300GetTsf32(struct ath_hal *ah)
{
    return OS_REG_READ(ah, AR_TSF_L32);
}

u_int32_t
ar9300GetTsf2_32(struct ath_hal *ah)
{
    return OS_REG_READ(ah, AR_TSF2_L32);
}

/*
 * Reset the current hardware tsf for stamlme.
 */
void
ar9300ResetTsf(struct ath_hal *ah)
{
    int count;

    count = 0;
    while (OS_REG_READ(ah, AR_SLP32_MODE) & AR_SLP32_TSF_WRITE_STATUS) {
        count++;
        if (count > 10) {
            HDPRINTF(ah, HAL_DBG_RESET, "%s: AR_SLP32_TSF_WRITE_STATUS limit exceeded\n", __func__);
            break;
        }
        OS_DELAY(10);
    }
    OS_REG_WRITE(ah, AR_RESET_TSF, AR_RESET_TSF_ONCE);
}

/*
 * Set or clear hardware basic rate bit
 * Set hardware basic rate set if basic rate is found
 * and basic rate is equal or less than 2Mbps
 */
void
ar9300SetBasicRate(struct ath_hal *ah, HAL_RATE_SET *rs)
{
    HAL_CHANNEL_INTERNAL *chan = AH_PRIVATE(ah)->ah_curchan;
    u_int32_t reg;
    u_int8_t xset;
    int i;

    if (chan == AH_NULL || !IS_CHAN_CCK(chan))
        return;
    xset = 0;
    for (i = 0; i < rs->rs_count; i++) {
        u_int8_t rset = rs->rs_rates[i];
        /* Basic rate defined? */
        if ((rset & 0x80) && (rset &= 0x7f) >= xset)
            xset = rset;
    }
    /*
     * Set the h/w bit to reflect whether or not the basic
     * rate is found to be equal or less than 2Mbps.
     */
    reg = OS_REG_READ(ah, AR_STA_ID1);
    if (xset && xset/2 <= 2) {
        OS_REG_WRITE(ah, AR_STA_ID1, reg | AR_STA_ID1_BASE_RATE_11B);
    } else {
        OS_REG_WRITE(ah, AR_STA_ID1, reg &~ AR_STA_ID1_BASE_RATE_11B);
    }
}

/*
 * Grab a semi-random value from hardware registers - may not
 * change often
 */
u_int32_t
ar9300GetRandomSeed(struct ath_hal *ah)
{
    u_int32_t nf;

    nf = (OS_REG_READ(ah, AR_PHY(25)) >> 19) & 0x1ff;
    if (nf & 0x100)
        nf = 0 - ((nf ^ 0x1ff) + 1);
    return (OS_REG_READ(ah, AR_TSF_U32) ^
        OS_REG_READ(ah, AR_TSF_L32) ^ nf);
}

/*
 * Detect if our card is present
 */
HAL_BOOL
ar9300DetectCardPresent(struct ath_hal *ah)
{
    u_int16_t macVersion, macRev;
    u_int32_t v;

    /*
     * Read the Silicon Revision register and compare that
     * to what we read at attach time.  If the same, we say
     * a card/device is present.
     */
    v = OS_REG_READ(ah, AR_HOSTIF_REG(ah, AR_SREV)) & AR_SREV_ID;
    if (v == 0xFF) {
        /* new SREV format */
        v = OS_REG_READ(ah, AR_HOSTIF_REG(ah, AR_SREV));
        /* Include 6-bit Chip Type (masked to 0) to differentiate from pre-Sowl versions */
        macVersion= (v & AR_SREV_VERSION2) >> AR_SREV_TYPE2_S;
        macRev = MS(v, AR_SREV_REVISION2);
    } else {
        macVersion = MS(v, AR_SREV_VERSION);
        macRev = v & AR_SREV_REVISION;
    }
    return (AH_PRIVATE(ah)->ah_macVersion == macVersion &&
            AH_PRIVATE(ah)->ah_macRev == macRev);
}

/*
 * Update MIB Counters
 */
void
ar9300UpdateMibMacStats(struct ath_hal *ah)
{
    struct ath_hal_9300 *ahp = AH9300(ah);
    HAL_MIB_STATS* stats = &ahp->ah_stats.ast_mibstats;

    stats->ackrcv_bad += OS_REG_READ(ah, AR_ACK_FAIL);
    stats->rts_bad    += OS_REG_READ(ah, AR_RTS_FAIL);
    stats->fcs_bad    += OS_REG_READ(ah, AR_FCS_FAIL);
    stats->rts_good   += OS_REG_READ(ah, AR_RTS_OK);
    stats->beacons    += OS_REG_READ(ah, AR_BEACON_CNT);
}

void
ar9300GetMibMacStats(struct ath_hal *ah, HAL_MIB_STATS* stats)
{
    struct ath_hal_9300 *ahp = AH9300(ah);
    HAL_MIB_STATS* istats = &ahp->ah_stats.ast_mibstats;

    stats->ackrcv_bad = istats->ackrcv_bad;
    stats->rts_bad    = istats->rts_bad;
    stats->fcs_bad    = istats->fcs_bad;
    stats->rts_good   = istats->rts_good;
    stats->beacons    = istats->beacons;
}

/*
 * Detect if the HW supports spreading a CCK signal on channel 14
 */
HAL_BOOL
ar9300IsJapanChannelSpreadSupported(struct ath_hal *ah)
{
    return AH_TRUE;
}

/*
 * Get the rssi of frame curently being received.
 */
u_int32_t
ar9300GetCurRssi(struct ath_hal *ah)
{
    // XXX return (OS_REG_READ(ah, AR_PHY_CURRENT_RSSI) & 0xff);
    /* get combined RSSI */
    return (OS_REG_READ(ah, AR_PHY_RSSI_3) & 0xff);
}

u_int
ar9300GetDefAntenna(struct ath_hal *ah)
{
    return (OS_REG_READ(ah, AR_DEF_ANTENNA) & 0x7);
}

/* Setup coverage class */
void
ar9300SetCoverageClass(struct ath_hal *ah, u_int8_t coverageclass, int now)
{
}

void
ar9300SetDefAntenna(struct ath_hal *ah, u_int antenna)
{
    OS_REG_WRITE(ah, AR_DEF_ANTENNA, (antenna & 0x7));
}

HAL_BOOL
ar9300SetAntennaSwitch(struct ath_hal *ah,
    HAL_ANT_SETTING settings, HAL_CHANNEL *chan, u_int8_t *tx_chainmask,
    u_int8_t *rx_chainmask, u_int8_t *antenna_cfgd)
{
    struct ath_hal_9300 *ahp = AH9300(ah);

    /*
     * Owl does not support diversity or changing antennas.
     *
     * Instead this API and function are defined differently for AR9300.
     * To support Tablet PC's, this interface allows the system
     * to dramatically reduce the TX power on a particular chain.
     *
     * Based on the value of (redefined) diversityControl, the
     * reset code will decrease power on chain 0 or chain 1/2.
     *
     * Based on the value of bit 0 of antennaSwitchSwap, the mapping between OID call
     * and chain is defined as:
     *  0:  map A -> 0, B -> 1;
     *  1:  map A -> 1, B -> 0;
     *
     * NOTE:
     *   The devices that use this OID should use a txChainMask and
     *   txChainSelectLegacy setting of 5 or 3 if ANTENNA_FIXED_B is
     *   used in order to ensure an active transmit antenna.  This
     *   API will allow the host to turn off the only transmitting
     *   antenna to ensure the antenna closest to the user's body is
     *   powered-down.
     */

    /* Set antenna control for use during reset sequence by ar9300DecreaseChainPower() */
    ahp->ah_diversityControl = settings;

    return AH_TRUE;
}

HAL_BOOL
ar9300IsSleepAfterBeaconBroken(struct ath_hal *ah)
{
    return AH_TRUE;
}

HAL_BOOL
ar9300SetSlotTime(struct ath_hal *ah, u_int us)
{
    struct ath_hal_9300 *ahp = AH9300(ah);
    if (us < HAL_SLOT_TIME_9 || us > ar9300MacToUsec(ah, 0xffff)) {
        HDPRINTF(ah, HAL_DBG_RESET, "%s: bad slot time %u\n", __func__, us);
        ahp->ah_slottime = (u_int) -1;  /* restore default handling */
        return AH_FALSE;
    } else {
        /* convert to system clocks */
        OS_REG_WRITE(ah, AR_D_GBL_IFS_SLOT, ar9300MacToClks(ah, us));
        ahp->ah_slottime = us;
        return AH_TRUE;
    }
}

u_int
ar9300GetSlotTime(struct ath_hal *ah)
{
    u_int clks = OS_REG_READ(ah, AR_D_GBL_IFS_SLOT) & 0xffff;
    return ar9300MacToUsec(ah, clks);      /* convert from system clocks */
}

HAL_BOOL
ar9300SetAckTimeout(struct ath_hal *ah, u_int us)
{
#ifdef AR9300_EMULATION
        OS_REG_RMW_FIELD(ah, AR_TIME_OUT, AR_TIME_OUT_ACK, 0x10f8);
        return AH_TRUE;
#else
    struct ath_hal_9300 *ahp = AH9300(ah);

    if (us > ar9300MacToUsec(ah, MS(0xffffffff, AR_TIME_OUT_ACK))) {
        HDPRINTF(ah, HAL_DBG_RESET, "%s: bad ack timeout %u\n", __func__, us);
        ahp->ah_acktimeout = (u_int) -1; /* restore default handling */
        return AH_FALSE;
    } else {
        /* convert to system clocks */
        OS_REG_RMW_FIELD(ah, AR_TIME_OUT, AR_TIME_OUT_ACK, ar9300MacToClks(ah, us));
        ahp->ah_acktimeout = us;
        return AH_TRUE;
    }
#endif
}

u_int
ar9300GetAckTimeout(struct ath_hal *ah)
{
    u_int clks = MS(OS_REG_READ(ah, AR_TIME_OUT), AR_TIME_OUT_ACK);
    return ar9300MacToUsec(ah, clks);      /* convert from system clocks */
}

HAL_BOOL
ar9300SetCTSTimeout(struct ath_hal *ah, u_int us)
{
#ifdef AR9300_EMULATION
        OS_REG_RMW_FIELD(ah, AR_TIME_OUT, AR_TIME_OUT_CTS, 0x10f8);
        return AH_TRUE;
#else
    struct ath_hal_9300 *ahp = AH9300(ah);

    if (us > ar9300MacToUsec(ah, MS(0xffffffff, AR_TIME_OUT_CTS))) {
        HDPRINTF(ah, HAL_DBG_RESET, "%s: bad cts timeout %u\n", __func__, us);
        ahp->ah_ctstimeout = (u_int) -1; /* restore default handling */
        return AH_FALSE;
    } else {
        /* convert to system clocks */
        OS_REG_RMW_FIELD(ah, AR_TIME_OUT, AR_TIME_OUT_CTS, ar9300MacToClks(ah, us));
        ahp->ah_ctstimeout = us;
        return AH_TRUE;
    }
#endif
}

u_int
ar9300GetCTSTimeout(struct ath_hal *ah)
{
    u_int clks = MS(OS_REG_READ(ah, AR_TIME_OUT), AR_TIME_OUT_CTS);
    return ar9300MacToUsec(ah, clks);      /* convert from system clocks */
}

HAL_STATUS
ar9300SetQuiet(struct ath_hal *ah,u_int16_t period, u_int16_t duration, u_int16_t nextStart, u_int16_t enabled)
{
    OS_REG_WRITE(ah, AR_QUIET2, period | (duration << AR_QUIET2_QUIET_DUR_S));
    OS_REG_WRITE(ah, AR_QUIET1, nextStart | (enabled << 16));

    return HAL_OK;
}

void
ar9300SetPCUConfig(struct ath_hal *ah)
{
    ar9300SetOperatingMode(ah, AH_PRIVATE(ah)->ah_opmode);
}

HAL_STATUS
ar9300GetCapability(struct ath_hal *ah, HAL_CAPABILITY_TYPE type,
        u_int32_t capability, u_int32_t *result)
{
    struct ath_hal_9300 *ahp = AH9300(ah);
    const HAL_CAPABILITIES *pCap = &AH_PRIVATE(ah)->ah_caps;

    switch (type) {
    case HAL_CAP_CIPHER:            /* cipher handled in hardware */
        switch (capability) {
        case HAL_CIPHER_AES_CCM:
        case HAL_CIPHER_AES_OCB:
        case HAL_CIPHER_TKIP:
        case HAL_CIPHER_WEP:
        case HAL_CIPHER_MIC:
        case HAL_CIPHER_CLR:
            return HAL_OK;
#if ATH_SUPPORT_WAPI
        case HAL_CIPHER_WAPI:
            return HAL_OK;
#endif			
        default:
            return HAL_ENOTSUPP;
        }
#if ATH_SUPPORT_WAPI
    case HAL_CAP_WAPI_MIC:
        return HAL_OK;
#endif		
    case HAL_CAP_TKIP_MIC:          /* handle TKIP MIC in hardware */
        switch (capability) {
        case 0:         /* hardware capability */
            return HAL_OK;
        case 1:
            return (ahp->ah_staId1Defaults &
                    AR_STA_ID1_CRPT_MIC_ENABLE) ?  HAL_OK : HAL_ENXIO;
        }
    case HAL_CAP_TKIP_SPLIT:        /* hardware TKIP uses split keys */
        /* XXX check rev when new parts are available */
        return (ahp->ah_miscMode & AR_PCU_MIC_NEW_LOC_ENA) ?  HAL_ENXIO : HAL_OK;
    case HAL_CAP_WME_TKIPMIC:   /* hardware can do TKIP MIC when WMM is turned on */
        return HAL_OK;
    case HAL_CAP_PHYCOUNTERS:       /* hardware PHY error counters */
        return ahp->ah_hasHwPhyCounters ? HAL_OK : HAL_ENXIO;
    case HAL_CAP_DIVERSITY:         /* hardware supports fast diversity */
        switch (capability) {
        case 0:                 /* hardware capability */
            return HAL_OK;
        case 1:                 /* current setting */
            return (OS_REG_READ(ah, AR_PHY_CCK_DETECT) &
                            AR_PHY_CCK_DETECT_BB_ENABLE_ANT_FAST_DIV) ?
                            HAL_OK : HAL_ENXIO;
        }
        return HAL_EINVAL;
    case HAL_CAP_TPC:
        switch (capability) {
        case 0:                 /* hardware capability */
            return HAL_OK;
        case 1:
            return AH_PRIVATE(ah)->ah_config.ath_hal_desc_tpc ?
                               HAL_OK : HAL_ENXIO;
        }
        return HAL_OK;
    case HAL_CAP_PHYDIAG:           /* radar pulse detection capability */
        return HAL_OK;
    case HAL_CAP_MCAST_KEYSRCH:     /* multicast frame keycache search */
        switch (capability) {
        case 0:                 /* hardware capability */
            return HAL_OK;
        case 1:
            if (OS_REG_READ(ah, AR_STA_ID1) & AR_STA_ID1_ADHOC) {
                /*
                 * Owl and Merlin have problems in mcast key search.
                 * Disable this cap. in Ad-hoc mode. see Bug 25776 and
                 * 26802
                 */
                return HAL_ENXIO;
            } else {
                return (ahp->ah_staId1Defaults &
                        AR_STA_ID1_MCAST_KSRCH) ? HAL_OK : HAL_ENXIO;
            }
        }
        return HAL_EINVAL;
    case HAL_CAP_TSF_ADJUST:        /* hardware has beacon tsf adjust */
        switch (capability) {
        case 0:                 /* hardware capability */
            return pCap->halTsfAddSupport ? HAL_OK : HAL_ENOTSUPP;
        case 1:
            return (ahp->ah_miscMode & AR_PCU_TX_ADD_TSF) ?  HAL_OK : HAL_ENXIO;
        }
        return HAL_EINVAL;
    case HAL_CAP_RFSILENT:      /* rfsilent support  */
        if (capability == 3) {  /* rfkill interrupt */
            /*
             * XXX: Interrupt-based notification of RF Kill state
             *      changes not working yet. Report that this feature
             *      is not supported so that polling is used instead.
             */
            return (HAL_ENOTSUPP);
        }
        return ath_hal_getcapability(ah, type, capability, result);
    case HAL_CAP_4ADDR_AGGR:
        return HAL_OK;
    case HAL_CAP_BB_RIFS_HANG:
        return HAL_ENOTSUPP;
    case HAL_CAP_BB_DFS_HANG:
        return HAL_ENOTSUPP;
    case HAL_CAP_BB_RX_CLEAR_STUCK_HANG:
        /* Track chips that are known to have BB hangs related
         * to rx_clear stuck low.
         */
        return HAL_ENOTSUPP;
    case HAL_CAP_MAC_HANG:
        /* Track chips that are known to have MAC hangs.
         */
        return HAL_ENOTSUPP;
    case HAL_CAP_RIFS_RX_ENABLED:
        /* Is RIFS RX currently enabled */
        return (ahp->ah_rifs_enabled == AH_TRUE) ?  HAL_OK : HAL_ENOTSUPP;
    case HAL_CAP_ANT_CFG_2GHZ:
        *result = pCap->halNumAntCfg2GHz;
        return HAL_OK;
    case HAL_CAP_ANT_CFG_5GHZ:
        *result = pCap->halNumAntCfg5GHz;
        return HAL_OK;
    case HAL_CAP_RX_STBC:
        *result = pCap->halRxStbcSupport;
        return HAL_OK;
    case HAL_CAP_TX_STBC:
        *result = pCap->halTxStbcSupport;
        return HAL_OK;
    case HAL_CAP_LDPC:
        *result = pCap->halLdpcSupport;
        return HAL_OK;
    case HAL_CAP_DYNAMIC_SMPS:
        return HAL_OK;
    case HAL_CAP_DS: /* supports both two and three streams */
        return (AR_SREV_HORNET(ah) || AR_SREV_POSEIDON(ah)) ? 
            HAL_ENOTSUPP : HAL_OK;
    case HAL_CAP_TS:
        return (AR_SREV_HORNET(ah) || AR_SREV_POSEIDON(ah) || 
            AR_SREV_WASP(ah) ) ? HAL_ENOTSUPP : HAL_OK;
    case HAL_CAP_OL_PWRCTRL:
        return (ar9300EepromGet(ahp, EEP_OL_PWRCTRL)) ?  HAL_OK : HAL_ENOTSUPP;
    case HAL_CAP_MAX_WEP_TKIP_HT20_TX_RATEKBPS:
        *result = (u_int32_t)(-1);
        return HAL_OK;
    case HAL_CAP_MAX_WEP_TKIP_HT40_TX_RATEKBPS:
        *result = (u_int32_t)(-1);
        return HAL_OK;
    case HAL_CAP_BB_PANIC_WATCHDOG:
        return HAL_OK;
#ifdef ATH_SUPPORT_TxBF
    case HAL_CAP_TXBF:
        return pCap->halTxBFSupport ? HAL_OK : HAL_ENOTSUPP;
#endif
    case HAL_CAP_PHYRESTART_CLR_WAR:
        return HAL_OK;
    case HAL_CAP_ENTERPRISE_MODE:
        *result = ahp->ah_enterprise_mode >> 16;
        return HAL_OK;
    case HAL_CAP_LDPCWAR:
        if ((AH_PRIVATE((ah))->ah_macVersion == AR_SREV_VERSION_OSPREY) &&
            (AH_PRIVATE((ah))->ah_macRev <= AR_SREV_REVISION_OSPREY_22)) 
        {
          return HAL_OK;    
        }
        else
        {
          return HAL_ENOTSUPP;
        }
    case HAL_CAP_ENABLE_APM:
        *result = pCap->halEnableApm;
        return HAL_OK;
    case HAL_CAP_PCIE_LCR_EXTSYNC_EN:
        return (pCap->halPcieLcrExtsyncEn == AH_TRUE) ?  HAL_OK : HAL_ENOTSUPP;
    case HAL_CAP_PCIE_LCR_OFFSET:
        *result = pCap->halPcieLcrOffset;
        return HAL_OK;
    default:
        return ath_hal_getcapability(ah, type, capability, result);
    }
}

HAL_BOOL
ar9300SetCapability(struct ath_hal *ah, HAL_CAPABILITY_TYPE type,
        u_int32_t capability, u_int32_t setting, HAL_STATUS *status)
{
    struct ath_hal_9300 *ahp = AH9300(ah);
    const HAL_CAPABILITIES *pCap = &AH_PRIVATE(ah)->ah_caps;
    u_int32_t v;

    switch (type) {
#if ATH_SUPPORT_WAPI
    case HAL_CAP_WAPI_MIC:
        ahp->ah_staId1Defaults |= AR_STA_ID1_CRPT_MIC_ENABLE;
        return AH_TRUE;
#endif		
    case HAL_CAP_TKIP_MIC:          /* handle TKIP MIC in hardware */
        if (setting)
            ahp->ah_staId1Defaults |= AR_STA_ID1_CRPT_MIC_ENABLE;
        else
            ahp->ah_staId1Defaults &= ~AR_STA_ID1_CRPT_MIC_ENABLE;
        return AH_TRUE;
    case HAL_CAP_DIVERSITY:
        v = OS_REG_READ(ah, AR_PHY_CCK_DETECT);
        if (setting)
            v |= AR_PHY_CCK_DETECT_BB_ENABLE_ANT_FAST_DIV;
        else
            v &= ~AR_PHY_CCK_DETECT_BB_ENABLE_ANT_FAST_DIV;
        OS_REG_WRITE(ah, AR_PHY_CCK_DETECT, v);
        return AH_TRUE;
    case HAL_CAP_DIAG:              /* hardware diagnostic support */
        /*
         * NB: could split this up into virtual capabilities,
         *     (e.g. 1 => ACK, 2 => CTS, etc.) but it hardly
         *     seems worth the additional complexity.
         */
#ifdef AH_DEBUG
        AH_PRIVATE(ah)->ah_diagreg = setting;
#else
        AH_PRIVATE(ah)->ah_diagreg = setting & 0x6;     /* ACK+CTS */
#endif
        OS_REG_WRITE(ah, AR_DIAG_SW, AH_PRIVATE(ah)->ah_diagreg);
        return AH_TRUE;
    case HAL_CAP_TPC:
        AH_PRIVATE(ah)->ah_config.ath_hal_desc_tpc = (setting != 0);
        return AH_TRUE;
    case HAL_CAP_MCAST_KEYSRCH:     /* multicast frame keycache search */
        if (setting)
            ahp->ah_staId1Defaults |= AR_STA_ID1_MCAST_KSRCH;
        else
            ahp->ah_staId1Defaults &= ~AR_STA_ID1_MCAST_KSRCH;
        return AH_TRUE;
    case HAL_CAP_TSF_ADJUST:        /* hardware has beacon tsf adjust */
        if (pCap->halTsfAddSupport) {
            if (setting)
                ahp->ah_miscMode |= AR_PCU_TX_ADD_TSF;
            else
                ahp->ah_miscMode &= ~AR_PCU_TX_ADD_TSF;
            return AH_TRUE;
        }
    case HAL_CAP_RXBUFSIZE:         /* set MAC receive buffer size */
        ahp->rxBufSize = setting & AR_DATABUF_MASK;
        OS_REG_WRITE(ah, AR_DATABUF, ahp->rxBufSize);
        return AH_TRUE;

        /* fall thru... */
    default:
        return ath_hal_setcapability(ah, type, capability, setting, status);
    }
}

#ifdef AH_DEBUG
static void
ar9300PrintReg(struct ath_hal *ah, u_int32_t args)
{
    u_int32_t i=0;

    /* Read 0x80d0 to trigger pcie analyzer */
    HDPRINTF(ah, HAL_DBG_PRINT_REG,"0x%04x 0x%08x\n", 0x80d0, OS_REG_READ(ah, 0x80d0));

    if (args & HAL_DIAG_PRINT_REG_COUNTER) {
        struct ath_hal_9300 *ahp = AH9300(ah);
        u_int32_t tf, rf, rc, cc;

        tf = OS_REG_READ(ah, AR_TFCNT);
        rf = OS_REG_READ(ah, AR_RFCNT);
        rc = OS_REG_READ(ah, AR_RCCNT);
        cc = OS_REG_READ(ah, AR_CCCNT);

        HDPRINTF(ah, HAL_DBG_PRINT_REG, "AR_TFCNT Diff= 0x%x\n", tf - ahp->lasttf);
        HDPRINTF(ah, HAL_DBG_PRINT_REG, "AR_RFCNT Diff= 0x%x\n", rf - ahp->lastrf);
        HDPRINTF(ah, HAL_DBG_PRINT_REG, "AR_RCCNT Diff= 0x%x\n", rc - ahp->lastrc);
        HDPRINTF(ah, HAL_DBG_PRINT_REG, "AR_CCCNT Diff= 0x%x\n", cc - ahp->lastcc);

        ahp->lasttf = tf;
        ahp->lastrf = rf;
        ahp->lastrc = rc;
        ahp->lastcc = cc;

        HDPRINTF(ah, HAL_DBG_PRINT_REG, "DMADBG0 = 0x%x\n", OS_REG_READ(ah, AR_DMADBG_0));
        HDPRINTF(ah, HAL_DBG_PRINT_REG, "DMADBG1 = 0x%x\n", OS_REG_READ(ah, AR_DMADBG_1));
        HDPRINTF(ah, HAL_DBG_PRINT_REG, "DMADBG2 = 0x%x\n", OS_REG_READ(ah, AR_DMADBG_2));
        HDPRINTF(ah, HAL_DBG_PRINT_REG, "DMADBG3 = 0x%x\n", OS_REG_READ(ah, AR_DMADBG_3));
        HDPRINTF(ah, HAL_DBG_PRINT_REG, "DMADBG4 = 0x%x\n", OS_REG_READ(ah, AR_DMADBG_4));
        HDPRINTF(ah, HAL_DBG_PRINT_REG, "DMADBG5 = 0x%x\n", OS_REG_READ(ah, AR_DMADBG_5));
        HDPRINTF(ah, HAL_DBG_PRINT_REG, "DMADBG6 = 0x%x\n", OS_REG_READ(ah, AR_DMADBG_6));
        HDPRINTF(ah, HAL_DBG_PRINT_REG, "DMADBG7 = 0x%x\n", OS_REG_READ(ah, AR_DMADBG_7));
    }

    if (args & HAL_DIAG_PRINT_REG_ALL) {
        for(i=0x8; i<=0xB8; i += sizeof(u_int32_t))
        {
            HDPRINTF(ah, HAL_DBG_PRINT_REG, "0x%04x 0x%08x\n", i, OS_REG_READ(ah, i));
        }

        for(i=0x800; i<=(0x800 + (10 << 2)); i += sizeof(u_int32_t))
        {
            HDPRINTF(ah, HAL_DBG_PRINT_REG, "0x%04x 0x%08x\n", i, OS_REG_READ(ah, i));
        }

        HDPRINTF(ah, HAL_DBG_PRINT_REG, "0x%04x 0x%08x\n", 0x840, OS_REG_READ(ah, i));

        HDPRINTF(ah, HAL_DBG_PRINT_REG, "0x%04x 0x%08x\n", 0x880, OS_REG_READ(ah, i));

        for(i=0x8C0; i<=(0x8C0 + (10 << 2)); i += sizeof(u_int32_t))
        {
            HDPRINTF(ah, HAL_DBG_PRINT_REG, "0x%04x 0x%08x\n", i, OS_REG_READ(ah, i));
        }

        for(i=0x1F00; i<=0x1F04; i += sizeof(u_int32_t))
        {
            HDPRINTF(ah, HAL_DBG_PRINT_REG, "0x%04x 0x%08x\n", i, OS_REG_READ(ah, i));
        }

        for(i=0x4000; i<=0x408C; i += sizeof(u_int32_t))
        {
            HDPRINTF(ah, HAL_DBG_PRINT_REG, "0x%04x 0x%08x\n", i, OS_REG_READ(ah, i));
        }

        for(i=0x5000; i<=0x503C; i += sizeof(u_int32_t))
        {
            HDPRINTF(ah, HAL_DBG_PRINT_REG, "0x%04x 0x%08x\n", i, OS_REG_READ(ah, i));
        }

        for(i=0x7040; i<=0x7058; i += sizeof(u_int32_t))
        {
            HDPRINTF(ah, HAL_DBG_PRINT_REG, "0x%04x 0x%08x\n", i, OS_REG_READ(ah, i));
        }

        for(i=0x8000; i<=0x8098; i += sizeof(u_int32_t))
        {
            HDPRINTF(ah, HAL_DBG_PRINT_REG, "0x%04x 0x%08x\n", i, OS_REG_READ(ah, i));
        }

        for(i=0x80D4; i<=0x8200; i += sizeof(u_int32_t))
        {
            HDPRINTF(ah, HAL_DBG_PRINT_REG, "0x%04x 0x%08x\n", i, OS_REG_READ(ah, i));
        }

        for(i=0x8240; i<=0x97FC; i += sizeof(u_int32_t))
        {
            HDPRINTF(ah, HAL_DBG_PRINT_REG, "0x%04x 0x%08x\n", i, OS_REG_READ(ah, i));
        }

        for(i=0x9800; i<=0x99f0; i += sizeof(u_int32_t))
        {
            HDPRINTF(ah, HAL_DBG_PRINT_REG, "0x%04x 0x%08x\n", i, OS_REG_READ(ah, i));
        }

        for(i=0x9c10; i<=0x9CFC; i += sizeof(u_int32_t))
        {
            HDPRINTF(ah, HAL_DBG_PRINT_REG, "0x%04x 0x%08x\n", i, OS_REG_READ(ah, i));
        }

        for(i=0xA200; i<=0xA26C; i += sizeof(u_int32_t))
        {
            HDPRINTF(ah, HAL_DBG_PRINT_REG, "0x%04x 0x%08x\n", i, OS_REG_READ(ah, i));
        }
    }
}
#endif

HAL_BOOL
ar9300GetDiagState(struct ath_hal *ah, int request,
        const void *args, u_int32_t argsize,
        void **result, u_int32_t *resultsize)
{
    struct ath_hal_9300 *ahp = AH9300(ah);

    (void) ahp;
    if (ath_hal_getdiagstate(ah, request, args, argsize, result, resultsize))
        return AH_TRUE;

    switch (request) {
#ifdef AH_PRIVATE_DIAG
    case HAL_DIAG_EEPROM:
        *result = &ahp->ah_eeprom;
        *resultsize = sizeof(HAL_EEPROM);
        return AH_TRUE;

#if 0   // XXX - TODO
        case HAL_DIAG_EEPROM_EXP_11A:
        case HAL_DIAG_EEPROM_EXP_11B:
        case HAL_DIAG_EEPROM_EXP_11G:
                pe = &ahp->ah_modePowerArray2133[
                        request - HAL_DIAG_EEPROM_EXP_11A];
                *result = pe->pChannels;
                *resultsize = (*result == AH_NULL) ? 0 :
                        roundup(sizeof(u_int16_t) * pe->numChannels,
                                sizeof(u_int32_t)) +
                        sizeof(EXPN_DATA_PER_CHANNEL_2133) * pe->numChannels;
                return AH_TRUE;
#endif
    case HAL_DIAG_RFGAIN:
        *result = &ahp->ah_gainValues;
        *resultsize = sizeof(GAIN_VALUES);
        return AH_TRUE;
    case HAL_DIAG_RFGAIN_CURSTEP:
        *result = (void *) ahp->ah_gainValues.currStep;
        *resultsize = (*result == AH_NULL) ?  0 : sizeof(GAIN_OPTIMIZATION_STEP);
                return AH_TRUE;
#if 0   // XXX - TODO
        case HAL_DIAG_PCDAC:
                *result = ahp->ah_pcdacTable;
                *resultsize = ahp->ah_pcdacTableSize;
                return AH_TRUE;
#endif
    case HAL_DIAG_TXRATES:
        *result = &ahp->ah_ratesArray[0];
        *resultsize = sizeof(ahp->ah_ratesArray);
        return AH_TRUE;
    case HAL_DIAG_ANI_CURRENT:
        *result = ar9300AniGetCurrentState(ah);
        *resultsize = (*result == AH_NULL) ?  0 : sizeof(struct ar9300AniState);
        return AH_TRUE;
    case HAL_DIAG_ANI_STATS:
        *result = ar9300AniGetCurrentStats(ah);
        *resultsize = (*result == AH_NULL) ?  0 : sizeof(struct ar9300Stats);
        return AH_TRUE;
    case HAL_DIAG_ANI_CMD:
        if (argsize != 2*sizeof(u_int32_t))
            return AH_FALSE;
        ar9300AniControl(ah, ((const u_int32_t *)args)[0], ((const u_int32_t *)args)[1]);
        return AH_TRUE;
    case HAL_DIAG_TXCONT:
        //AR9300_CONTTXMODE(ah, (struct ath_desc *)args, argsize );
        return AH_TRUE;
#endif /* AH_PRIVATE_DIAG */
#ifdef AH_DEBUG
    case HAL_DIAG_PRINT_REG:
        ar9300PrintReg(ah, *((u_int32_t *)args));
        return AH_TRUE;
#endif
    default:
        break;
    }

        return AH_FALSE;
}

void
ar9300DmaRegDump(struct ath_hal *ah)
{
#define NUM_DMA_DEBUG_REGS  8
#define NUM_QUEUES          10

    u_int32_t val[NUM_DMA_DEBUG_REGS];
    int       qcuOffset = 0, dcuOffset = 0;
    u_int32_t *qcuBase  = &val[0], *dcuBase = &val[4], reg;
    int       i, j, k;
    int16_t nfarray[NUM_NF_READINGS];
    HAL_NFCAL_HIST_FULL *h =
#ifdef ATH_NF_PER_CHAN
        &AH_PRIVATE(ah)->ah_curchan->nfCalHist;
#else
        &AH_PRIVATE(ah)->nfCalHist;
#endif

     /* selecting DMA OBS 8 */
    OS_REG_WRITE(ah,AR_MACMISC, 
        ((AR_MACMISC_DMA_OBS_LINE_8 << AR_MACMISC_DMA_OBS_S) | 
         (AR_MACMISC_MISC_OBS_BUS_1 << AR_MACMISC_MISC_OBS_BUS_MSB_S)));
 
    ath_hal_printf(ah, "Raw DMA Debug values:\n");
    for (i = 0; i < NUM_DMA_DEBUG_REGS; i++) {
        if (i % 4 == 0) {
            ath_hal_printf(ah, "\n");
        }

        val[i] = OS_REG_READ(ah, AR_DMADBG_0 + (i * sizeof(u_int32_t)));
        ath_hal_printf(ah, "%d: %08x ", i, val[i]);
    }

    ath_hal_printf(ah, "\n\n");
    ath_hal_printf(ah, "Num QCU: chain_st fsp_ok fsp_st DCU: chain_st\n");

    for (i = 0; i < NUM_QUEUES; i++, qcuOffset += 4, dcuOffset += 5) {
        if (i == 8) {
            /* only 8 QCU entries in val[0] */
            qcuOffset = 0;
            qcuBase++;
        }

        if (i == 6) {
            /* only 6 DCU entries in val[4] */
            dcuOffset = 0;
            dcuBase++;
        }

        ath_hal_printf(ah,  "%2d          %2x      %1x     %2x           %2x\n",
                  i,
                  (*qcuBase & (0x7 << qcuOffset)) >> qcuOffset,
                  (*qcuBase & (0x8 << qcuOffset)) >> (qcuOffset + 3),
                  val[2] & (0x7 << (i * 3)) >> (i * 3),
                  (*dcuBase & (0x1f << dcuOffset)) >> dcuOffset);
    }

    ath_hal_printf(ah, "\n");
    ath_hal_printf(ah, "qcu_stitch state:   %2x    qcu_fetch state:        %2x\n",
              (val[3] & 0x003c0000) >> 18, (val[3] & 0x03c00000) >> 22);
    ath_hal_printf(ah,  "qcu_complete state: %2x    dcu_complete state:     %2x\n",
              (val[3] & 0x1c000000) >> 26, (val[6] & 0x3));
    ath_hal_printf(ah,  "dcu_arb state:      %2x    dcu_fp state:           %2x\n",
             (val[5] & 0x06000000) >> 25, (val[5] & 0x38000000) >> 27);
    ath_hal_printf(ah,  "chan_idle_dur:     %3d    chan_idle_dur_valid:     %1d\n",
              (val[6] & 0x000003fc) >> 2, (val[6] & 0x00000400) >> 10);
    ath_hal_printf(ah,  "txfifo_valid_0:      %1d    txfifo_valid_1:          %1d\n",
              (val[6] & 0x00000800) >> 11, (val[6] & 0x00001000) >> 12);
    ath_hal_printf(ah,  "txfifo_dcu_num_0:   %2d    txfifo_dcu_num_1:       %2d\n",
              (val[6] & 0x0001e000) >> 13, (val[6] & 0x001e0000) >> 17);

    ath_hal_printf(ah, "pcu observe 0x%x \n", OS_REG_READ(ah, AR_OBS_BUS_1)); 
    ath_hal_printf(ah, "AR_CR 0x%x \n", OS_REG_READ(ah,AR_CR));

    ar9300UploadNoiseFloor(ah, 1, nfarray);
    ath_hal_printf(ah, "2G:\n");
    ath_hal_printf(ah, "Min CCA Out:\n");
    ath_hal_printf(ah, "\t\tChain 0\t\tChain 1\t\tChain 2\n");
    ath_hal_printf(ah, "Control:\t%8d\t%8d\t%8d\n",
                   nfarray[0], nfarray[1], nfarray[2]);
    ath_hal_printf(ah, "Extension:\t%8d\t%8d\t%8d\n\n",
                   nfarray[3], nfarray[4], nfarray[5]);

    ar9300UploadNoiseFloor(ah, 0, nfarray);
    ath_hal_printf(ah, "5G:\n");
    ath_hal_printf(ah, "Min CCA Out:\n");
    ath_hal_printf(ah, "\t\tChain 0\t\tChain 1\t\tChain 2\n");
    ath_hal_printf(ah, "Control:\t%8d\t%8d\t%8d\n",
                   nfarray[0], nfarray[1], nfarray[2]);
    ath_hal_printf(ah, "Extension:\t%8d\t%8d\t%8d\n\n",
                   nfarray[3], nfarray[4], nfarray[5]);

    for (i = 0; i < NUM_NF_READINGS; i++) {
        ath_hal_printf(ah, "%s Chain %d NF History:\n",
                       ((i < 3) ? "Control " : "Extension "), i%3);
        for (j=0, k = h->base.currIndex;
             j < HAL_NF_CAL_HIST_LEN_FULL;
             j++, k++) {
            ath_hal_printf(ah, "Element %d: %d\n",
                           j, h->nfCalBuffer[k % HAL_NF_CAL_HIST_LEN_FULL][i]);
        }
        ath_hal_printf(ah, "Last Programmed NF: %d\n\n", h->base.privNF[i]);
    }

    reg = OS_REG_READ(ah, AR_PHY_FIND_SIG_LOW);
    ath_hal_printf(ah, "FIRStep Low = 0x%x (%d)\n",
                   MS(reg, AR_PHY_FIND_SIG_LOW_FIRSTEP_LOW),
                   MS(reg, AR_PHY_FIND_SIG_LOW_FIRSTEP_LOW));
    reg = OS_REG_READ(ah, AR_PHY_DESIRED_SZ);
    ath_hal_printf(ah, "Total Desired = 0x%x (%d)\n",
                   MS(reg, AR_PHY_DESIRED_SZ_TOT_DES),
                   MS(reg, AR_PHY_DESIRED_SZ_TOT_DES));
    ath_hal_printf(ah, "ADC Desired = 0x%x (%d)\n",
                   MS(reg, AR_PHY_DESIRED_SZ_ADC),
                   MS(reg, AR_PHY_DESIRED_SZ_ADC));
    reg = OS_REG_READ(ah, AR_PHY_FIND_SIG);
    ath_hal_printf(ah, "FIRStep = 0x%x (%d)\n",
                   MS(reg, AR_PHY_FIND_SIG_FIRSTEP),
                   MS(reg, AR_PHY_FIND_SIG_FIRSTEP));
    reg = OS_REG_READ(ah, AR_PHY_AGC);
    ath_hal_printf(ah, "Coarse High = 0x%x (%d)\n",
                   MS(reg, AR_PHY_AGC_COARSE_HIGH),
                   MS(reg, AR_PHY_AGC_COARSE_HIGH));
    ath_hal_printf(ah, "Coarse Low = 0x%x (%d)\n",
                   MS(reg, AR_PHY_AGC_COARSE_LOW),
                   MS(reg, AR_PHY_AGC_COARSE_LOW));
    ath_hal_printf(ah, "Coarse Power Constant = 0x%x (%d)\n",
                   MS(reg, AR_PHY_AGC_COARSE_PWR_CONST),
                   MS(reg, AR_PHY_AGC_COARSE_PWR_CONST));
    reg = OS_REG_READ(ah, AR_PHY_TIMING5);
    ath_hal_printf(ah, "Enable Cyclic Power Thresh = %d\n",
                   MS(reg, AR_PHY_TIMING5_CYCPWR_THR1_ENABLE));
    ath_hal_printf(ah, "Cyclic Power Thresh = 0x%x (%d)\n",
                   MS(reg, AR_PHY_TIMING5_CYCPWR_THR1),
                   MS(reg, AR_PHY_TIMING5_CYCPWR_THR1));
    ath_hal_printf(ah, "Cyclic Power Thresh 1A= 0x%x (%d)\n",
                   MS(reg, AR_PHY_TIMING5_CYCPWR_THR1A),
                   MS(reg, AR_PHY_TIMING5_CYCPWR_THR1A));
    reg = OS_REG_READ(ah, AR_PHY_DAG_CTRLCCK);
    ath_hal_printf(ah, "Barker RSSI Thresh Enable = %d\n",
                   MS(reg, AR_PHY_DAG_CTRLCCK_EN_RSSI_THR));
    ath_hal_printf(ah, "Barker RSSI Thresh = 0x%x (%d)\n",
                   MS(reg, AR_PHY_DAG_CTRLCCK_RSSI_THR),
                   MS(reg, AR_PHY_DAG_CTRLCCK_RSSI_THR));


     /* Step 1a: Set bit 23 of register 0xa360 to 0 */
     reg = OS_REG_READ(ah, 0xa360);
     reg &= ~0x00800000;
     OS_REG_WRITE(ah, 0xa360, reg);

     /* Step 2a: Set register 0xa364 to 0x1000 */
     reg = 0x1000;
     OS_REG_WRITE(ah, 0xa364, reg);

     /* Step 3a: Read bits 17:0 of register 0x9c20 */
     reg = OS_REG_READ(ah, 0x9c20);
     reg &= 0x0003ffff;
     ath_hal_printf(ah, "%s: Test Control Status [0x1000] 0x9c20[17:0] = 0x%x\n", __func__, reg);

     /* Step 1b: Set bit 23 of register 0xa360 to 0 */
     reg = OS_REG_READ(ah, 0xa360);
     reg &= ~0x00800000;
     OS_REG_WRITE(ah, 0xa360, reg);

     /* Step 2b: Set register 0xa364 to 0x1400 */
     reg = 0x1400;
     OS_REG_WRITE(ah, 0xa364, reg);

     /* Step 3b: Read bits 17:0 of register 0x9c20 */
     reg = OS_REG_READ(ah, 0x9c20);
     reg &= 0x0003ffff;
     ath_hal_printf(ah, "%s: Test Control Status [0x1400] 0x9c20[17:0] = 0x%x\n", __func__, reg);

     /* Step 1c: Set bit 23 of register 0xa360 to 0 */
     reg = OS_REG_READ(ah, 0xa360);
     reg &= ~0x00800000;
     OS_REG_WRITE(ah, 0xa360, reg);

     /* Step 2c: Set register 0xa364 to 0x3C00 */
     reg = 0x3c00;
     OS_REG_WRITE(ah, 0xa364, reg);

     /* Step 3c: Read bits 17:0 of register 0x9c20 */
     reg = OS_REG_READ(ah, 0x9c20);
     reg &= 0x0003ffff;
     ath_hal_printf(ah, "%s: Test Control Status [0x3C00] 0x9c20[17:0] = 0x%x\n", __func__, reg);

     /* Step 1d: Set bit 24 of register 0xa360 to 0 */
     reg = OS_REG_READ(ah, 0xa360);
     reg &= ~0x001040000;
     OS_REG_WRITE(ah, 0xa360, reg);

     /* Step 2d: Set register 0xa364 to 0x5005D */
     reg = 0x5005D;
     OS_REG_WRITE(ah, 0xa364, reg);

     /* Step 3d: Read bits 17:0 of register 0xa368 */
     reg = OS_REG_READ(ah, 0xa368);
     reg &= 0x0003ffff;
     ath_hal_printf(ah, "%s: Test Control Status [0x5005D] 0xa368[17:0] = 0x%x\n", __func__, reg);

     /* Step 1e: Set bit 24 of register 0xa360 to 0 */
     reg = OS_REG_READ(ah, 0xa360);
     reg &= ~0x001040000;
     OS_REG_WRITE(ah, 0xa360, reg);

     /* Step 2e: Set register 0xa364 to 0x7005D */
     reg = 0x7005D;
     OS_REG_WRITE(ah, 0xa364, reg);

     /* Step 3e: Read bits 17:0 of register 0xa368 */
     reg = OS_REG_READ(ah, 0xa368);
     reg &= 0x0003ffff;
     ath_hal_printf(ah, "%s: Test Control Status [0x7005D] 0xa368[17:0] = 0x%x\n", __func__, reg);

     /* Step 1f: Set bit 24 of register 0xa360 to 0 */
     reg = OS_REG_READ(ah, 0xa360);
     reg &= ~0x001000000;
     reg |= 0x40000;
     OS_REG_WRITE(ah, 0xa360, reg);

     /* Step 2f: Set register 0xa364 to 0x3005D */
     reg = 0x3005D;
     OS_REG_WRITE(ah, 0xa364, reg);

     /* Step 3f: Read bits 17:0 of register 0xa368 */
     reg = OS_REG_READ(ah, 0xa368);
     reg &= 0x0003ffff;
     ath_hal_printf(ah, "%s: Test Control Status [0x3005D] 0xa368[17:0] = 0x%x\n", __func__, reg);

     /* Step 1g: Set bit 24 of register 0xa360 to 0 */
     reg = OS_REG_READ(ah, 0xa360);
     reg &= ~0x001000000;
     reg |= 0x40000;
     OS_REG_WRITE(ah, 0xa360, reg);

     /* Step 2g: Set register 0xa364 to 0x6005D */
     reg = 0x6005D;
     OS_REG_WRITE(ah, 0xa364, reg);

     /* Step 3g: Read bits 17:0 of register 0xa368 */
     reg = OS_REG_READ(ah, 0xa368);
     reg &= 0x0003ffff;
     ath_hal_printf(ah, "%s: Test Control Status [0x6005D] 0xa368[17:0] = 0x%x\n", __func__, reg);

}

/*
 * Return the busy for rx_frame, rx_clear, and tx_frame
 */
u_int32_t
ar9300GetMibCycleCountsPct(struct ath_hal *ah, u_int32_t *rxc_pcnt, u_int32_t *rxf_pcnt, u_int32_t *txf_pcnt)
{
    struct ath_hal_9300 *ahp = AH9300(ah);
    u_int32_t good = 1;

    u_int32_t rc = OS_REG_READ(ah, AR_RCCNT);
    u_int32_t rf = OS_REG_READ(ah, AR_RFCNT);
    u_int32_t tf = OS_REG_READ(ah, AR_TFCNT);
    u_int32_t cc = OS_REG_READ(ah, AR_CCCNT); /* read cycles last */

    if (ahp->ah_cycles == 0 || ahp->ah_cycles > cc) {
        /*
         * Cycle counter wrap (or initial call); it's not possible
         * to accurately calculate a value because the registers
         * right shift rather than wrap--so punt and return 0.
         */
        HDPRINTF(ah, HAL_DBG_CHANNEL, "%s: cycle counter wrap. ExtBusy = 0\n", __func__);
        good = 0;
    } else {
        u_int32_t cc_d = cc - ahp->ah_cycles;
        u_int32_t rc_d = rc - ahp->ah_rx_clear;
        u_int32_t rf_d = rf - ahp->ah_rx_frame;
        u_int32_t tf_d = tf - ahp->ah_tx_frame;

        if (cc_d != 0) {
            *rxc_pcnt = rc_d * 100 / cc_d;
            *rxf_pcnt = rf_d * 100 / cc_d;
            *txf_pcnt = tf_d * 100 / cc_d;
        } else {
            good = 0;
        }
    }

    ahp->ah_cycles = cc;
    ahp->ah_rx_frame = rf;
    ahp->ah_rx_clear = rc;
    ahp->ah_tx_frame = tf;

    return good;
}

/*
 * Return approximation of extension channel busy over an time interval
 * 0% (clear) -> 100% (busy)
 * -1 for invalid estimate 
 */
int8_t
ar9300Get11nExtBusy(struct ath_hal *ah)
{
/* Overflow condition to check before multiplying to get % (x * 100 > 0xFFFFFFFF ) => (x > 0x28F5C28) */
#define OVERFLOW_LIMIT  0x28F5C28
#define ERROR_CODE      -1    

    struct ath_hal_9300 *ahp = AH9300(ah);
    u_int32_t busy = 0; /* percentage */
    int8_t busyper = 0;
    u_int32_t cycleCount, ctlBusy, extBusy;

    /* cycleCount will always be the first to wrap; therefore, read it last
     * This sequence of reads is not atomic, and MIB counter wrap
     * could happen during it ?
     */
    ctlBusy = OS_REG_READ(ah, AR_RCCNT);
    extBusy = OS_REG_READ(ah, AR_EXTRCCNT);
    cycleCount = OS_REG_READ(ah, AR_CCCNT);

    if ((ahp->ah_cycleCount == 0) || (ahp->ah_cycleCount > cycleCount) ||
    (ahp->ah_ctlBusy > ctlBusy) ||(ahp->ah_extBusy > extBusy)) {
        /*
         * Cycle counter wrap (or initial call); it's not possible
         * to accurately calculate a value because the registers
         * right shift rather than wrap--so punt and return 0.
         */
        busyper = ERROR_CODE;
        HDPRINTF(ah, HAL_DBG_CHANNEL, "%s: cycle counter wrap. ExtBusy = 0\n", __func__);

    } else {
        u_int32_t cycleDelta = cycleCount - ahp->ah_cycleCount;
        u_int32_t extBusyDelta = extBusy - ahp->ah_extBusy;


        /* Compute extension channel busy percentage
         * Overflow condition: 0xFFFFFFFF < extBusyDelta * 100
         * Underflow condition/Divide-by-zero: check that cycleDelta >> 7 != 0
         * Will never happen, since (extBusyDelta < cycleDelta) always, and shift necessitated by
         * large extBusyDelta.
         * Due to timing difference to read the registers and counter overflow, it may still happen that
         * cycleDelta >> 7 = 0.
         *
         */
        if (cycleDelta) {
            if (extBusyDelta > OVERFLOW_LIMIT) {
                if (cycleDelta >> 7) {
                    busy = ((extBusyDelta >> 7) * 100) / (cycleDelta  >> 7);
                } else {
                    busyper = ERROR_CODE;
                }
            } else {
                busy = (extBusyDelta * 100) / cycleDelta;
            }
        } else {
            busyper = ERROR_CODE;
        }

        if (busy > 100) {
            busy = 100;
        }
        if ( busyper != ERROR_CODE ) {
            busyper = busy;
        }
    }

    ahp->ah_cycleCount = cycleCount;
    ahp->ah_ctlBusy = ctlBusy;
    ahp->ah_extBusy = extBusy;

    return busyper;
#undef OVERFLOW_LIMIT
#undef ERROR_CODE    
}

/* BB Panic Watchdog declarations */
#define HAL_BB_PANIC_WD_HT20_FACTOR         74  /* 0.74 */
#define HAL_BB_PANIC_WD_HT40_FACTOR         37  /* 0.37 */

void
ar9300ConfigBbPanicWatchdog(struct ath_hal *ah)
{
    HAL_CHANNEL_INTERNAL *chan = AH_PRIVATE(ah)->ah_curchan;
    u_int32_t idle_tmo_ms = AH_PRIVATE(ah)->ah_bbPanicTimeoutMs;
    u_int32_t val, idle_count;

    if (idle_tmo_ms != 0) {
        // enable IRQ, disable chip-reset for BB panic
        val = OS_REG_READ(ah, AR_PHY_PANIC_WD_CTL_2) & AR_PHY_BB_PANIC_CNTL2_MASK;
        OS_REG_WRITE(ah, AR_PHY_PANIC_WD_CTL_2, (val | AR_PHY_BB_PANIC_IRQ_ENABLE) & ~AR_PHY_BB_PANIC_RST_ENABLE);
        /* bound limit to 10 secs */
        if (idle_tmo_ms > 10000) {
            idle_tmo_ms = 10000;
        }
        if (chan != AH_NULL && IS_CHAN_HT40(chan)) {
            idle_count = (100 * idle_tmo_ms) / HAL_BB_PANIC_WD_HT40_FACTOR;
        } else {
            idle_count = (100 * idle_tmo_ms) / HAL_BB_PANIC_WD_HT20_FACTOR;
        }
        // enable panic in non-IDLE mode, disable in IDLE mode, set idle time-out
        OS_REG_WRITE(ah, AR_PHY_PANIC_WD_CTL_1, 
                     AR_PHY_BB_PANIC_NON_IDLE_ENABLE | 
                     AR_PHY_BB_PANIC_IDLE_MASK | (AR_PHY_BB_PANIC_NON_IDLE_MASK & (idle_count << 2)));
    } else {
        // disable IRQ, disable chip-reset for BB panic
        OS_REG_WRITE(ah, AR_PHY_PANIC_WD_CTL_2, 
                     OS_REG_READ(ah, AR_PHY_PANIC_WD_CTL_2) & ~(AR_PHY_BB_PANIC_RST_ENABLE | AR_PHY_BB_PANIC_IRQ_ENABLE));
        // disable panic in non-IDLE mode, disable in IDLE mode
        OS_REG_WRITE(ah, AR_PHY_PANIC_WD_CTL_1, 
                    OS_REG_READ(ah, AR_PHY_PANIC_WD_CTL_1) & ~(AR_PHY_BB_PANIC_NON_IDLE_ENABLE | AR_PHY_BB_PANIC_IDLE_ENABLE));
    }

    HDPRINTF(ah, HAL_DBG_UNMASKABLE, "%s: %s BB Panic Watchdog tmo=%ums\n", 
             __func__, idle_tmo_ms ? "Enabled" : "Disabled", idle_tmo_ms);
}


void
ar9300HandleBbPanic(struct ath_hal *ah)
{
    u_int32_t status;
    // we want to avoid printing in ISR context so we save 
    // panic watchdog status to be printed later in DPC context
    AH_PRIVATE(ah)->ah_bbPanicLastStatus = status = OS_REG_READ(ah, AR_PHY_PANIC_WD_STATUS);
    // panic watchdog timer should reset on status read but to make sure we write 0 to the watchdog status bit
    OS_REG_WRITE(ah, AR_PHY_PANIC_WD_STATUS, status & ~AR_PHY_BB_WD_STATUS_CLR);
}

void
ar9300PrintBbPanicInfo(struct ath_hal *ah)
{
    u_int32_t rxc_pcnt = 0, rxf_pcnt = 0, txf_pcnt = 0, status;

    status = AH_PRIVATE(ah)->ah_bbPanicLastStatus;

    /*
     * For signature 04000539 do not print anything.
     * This is a very common occurence as a compromise between
     * BB Panic and false detects (EV71009). It indicates 
     * radar hang, which can be cleared by reprogramming
     * radar related register and does not requre a chip reset 
     */

    if (status == 0x04000539) {
        return;
    }

    ath_hal_printf(ah, "\n==== BB update: BB status=0x%08x, tsf=0x%08x ====\n", 
                   status, ar9300GetTsf32(ah));
    ath_hal_printf(ah, "** BB state: wd=%u det=%u rdar=%u rOFDM=%d rCCK=%u tOFDM=%u tCCK=%u agc=%u src=%u **\n", 
                   MS(status,AR_PHY_BB_WD_STATUS), MS(status,AR_PHY_BB_WD_DET_HANG), MS(status,AR_PHY_BB_WD_RADAR_SM), 
                   MS(status,AR_PHY_BB_WD_RX_OFDM_SM), MS(status,AR_PHY_BB_WD_RX_CCK_SM), 
                   MS(status,AR_PHY_BB_WD_TX_OFDM_SM), MS(status,AR_PHY_BB_WD_TX_CCK_SM), 
                   MS(status,AR_PHY_BB_WD_AGC_SM), MS(status,AR_PHY_BB_WD_SRCH_SM));
    ath_hal_printf(ah, "** BB WD cntl: cntl1=0x%08x cntl2=0x%08x **\n", 
                   OS_REG_READ(ah, AR_PHY_PANIC_WD_CTL_1), OS_REG_READ(ah, AR_PHY_PANIC_WD_CTL_2));
    ath_hal_printf(ah, "** BB mode: BB_gen_controls=0x%08x **\n", 
                   OS_REG_READ(ah, AR_PHY_GEN_CTRL));
    if (ar9300GetMibCycleCountsPct(ah, &rxc_pcnt, &rxf_pcnt, &txf_pcnt)) {
        ath_hal_printf(ah, "** BB busy times: rx_clear=%d%%, rx_frame=%d%%, tx_frame=%d%% **\n",
                       rxc_pcnt, rxf_pcnt, txf_pcnt);
    }
    ath_hal_printf(ah, "==== BB update: done ====\n\n");
}

/* set the reason for HAL reset */
void 
ar9300SetHalResetReason(struct ath_hal *ah, u_int8_t resetreason)
{
    AH_PRIVATE(ah)->ah_reset_reason = resetreason;
}

/*
 * Configure 20/40 operation
 *
 * 20/40 = joint rx clear (control and extension)
 * 20    = rx clear (control)
 *
 * - NOTE: must stop MAC (tx) and requeue 40 MHz packets as 20 MHz when changing
 *         from 20/40 => 20 only
 */
void
ar9300Set11nMac2040(struct ath_hal *ah, HAL_HT_MACMODE mode)
{
    u_int32_t macmode;

    /* Configure MAC for 20/40 operation */
    if (mode == HAL_HT_MACMODE_2040 &&
        !AH_PRIVATE(ah)->ah_config.ath_hal_cwmIgnoreExtCCA) {
        macmode = AR_2040_JOINED_RX_CLEAR;
    } else {
        macmode = 0;
    }
    OS_REG_WRITE(ah, AR_2040_MODE, macmode);
}

/*
 * Get Rx clear (control/extension channel)
 *
 * Returns active low (busy) for ctrl/ext channel
 * Owl 2.0
 */
HAL_HT_RXCLEAR
ar9300Get11nRxClear(struct ath_hal *ah)
{
    HAL_HT_RXCLEAR rxclear = 0;
    u_int32_t val;

    val = OS_REG_READ(ah, AR_DIAG_SW);

    /* control channel */
    if (val & AR_DIAG_RX_CLEAR_CTL_LOW) {
        rxclear |= HAL_RX_CLEAR_CTL_LOW;
    }
    /* extension channel */
    if (val & AR_DIAG_RX_CLEAR_CTL_LOW) {
        rxclear |= HAL_RX_CLEAR_EXT_LOW;
    }
    return rxclear;
}

/*
 * Set Rx clear (control/extension channel)
 *
 * Useful for forcing the channel to appear busy for
 * debugging/diagnostics
 * Owl 2.0
 */
void
ar9300Set11nRxClear(struct ath_hal *ah, HAL_HT_RXCLEAR rxclear)
{
    /* control channel */
    if (rxclear & HAL_RX_CLEAR_CTL_LOW) {
        OS_REG_SET_BIT(ah, AR_DIAG_SW, AR_DIAG_RX_CLEAR_CTL_LOW);
    } else {
        OS_REG_CLR_BIT(ah, AR_DIAG_SW, AR_DIAG_RX_CLEAR_CTL_LOW);
    }
    /* extension channel */
    if (rxclear & HAL_RX_CLEAR_EXT_LOW) {
        OS_REG_SET_BIT(ah, AR_DIAG_SW, AR_DIAG_RX_CLEAR_EXT_LOW);
    } else {
        OS_REG_CLR_BIT(ah, AR_DIAG_SW, AR_DIAG_RX_CLEAR_EXT_LOW);
    }
}

#ifdef AR9300_EMULATION
/* MAC tracing (emulation only)
 * - stop tracing (ar9300StopMacTrace)
 *   and then use "dumpregs -l" to display MAC trace buffer
 */
void
ar9300InitMacTrace(struct ath_hal *ah)
{
    OS_REG_WRITE(ah, AR_MAC_PCU_LOGIC_ANALYZER_32L, 0xFFFFFFFF);
    OS_REG_WRITE(ah, AR_MAC_PCU_LOGIC_ANALYZER_16U, 0xFFFF);
}

void
ar9300StopMacTrace(struct ath_hal *ah)
{
    OS_REG_CLR_BIT(ah, AR_MAC_PCU_LOGIC_ANALYZER,
           (AR_MAC_PCU_LOGIC_ANALYZER_CTL | AR_MAC_PCU_LOGIC_ANALYZER_QCU_SEL));
    OS_REG_SET_BIT(ah, AR_MAC_PCU_LOGIC_ANALYZER, AR_MAC_PCU_LOGIC_ANALYZER_ENABLE | AR_MAC_PCU_LOGIC_ANALYZER_HOLD |
                        (1 << AR_MAC_PCU_LOGIC_ANALYZER_QCU_SEL_S));
}
#endif //AR9300_EMULATION

/*
 * HAL support code for force ppm tracking workaround.
 */

u_int32_t
ar9300PpmGetRssiDump(struct ath_hal *ah)
{
    u_int32_t retval;
    u_int32_t off1;
    u_int32_t off2;

    if (OS_REG_READ(ah, AR_PHY_ANALOG_SWAP) & AR_PHY_SWAP_ALT_CHAIN) {
        off1 = 0x2000;
        off2 = 0x1000;
    } else {
        off1 = 0x1000;
        off2 = 0x2000;
    }

    retval = ((0xff & OS_REG_READ(ah, AR_PHY_CHAN_INFO_GAIN_0       )) << 0) |
             ((0xff & OS_REG_READ(ah, AR_PHY_CHAN_INFO_GAIN_0 + off1)) << 8) |
             ((0xff & OS_REG_READ(ah, AR_PHY_CHAN_INFO_GAIN_0 + off2)) << 16);

    return retval;
}

u_int32_t
ar9300PpmForce(struct ath_hal *ah)
{
    u_int32_t data_fine;
    u_int32_t data4;
    u_int32_t off1;
    u_int32_t off2;
    HAL_BOOL signed_val = AH_FALSE;

    if (OS_REG_READ(ah, AR_PHY_ANALOG_SWAP) & AR_PHY_SWAP_ALT_CHAIN) {
        off1 = 0x2000;
        off2 = 0x1000;
    } else {
        off1 = 0x1000;
        off2 = 0x2000;
    }
    data_fine = AR_PHY_CHAN_INFO_GAIN_DIFF_PPM_MASK & OS_REG_READ(ah, AR_PHY_CHNINFO_GAINDIFF);

    /*
     * bit [11-0] is new ppm value. bit 11 is the signed bit.
     * So check value from bit[10:0].
     * Now get the abs val of the ppm value read in bit[0:11].
     * After that do bound check on abs value.
     * if value is off limit, CAP the value and and restore signed bit.
     */
    if (data_fine & AR_PHY_CHAN_INFO_GAIN_DIFF_PPM_SIGNED_BIT)
    {
         /* get the positive value */
         data_fine = (~data_fine + 1) & AR_PHY_CHAN_INFO_GAIN_DIFF_PPM_MASK;
         signed_val = AH_TRUE;
    }
    if (data_fine > AR_PHY_CHAN_INFO_GAIN_DIFF_UPPER_LIMIT)
    {
        HDPRINTF(ah, HAL_DBG_REG_IO, "%s Correcting ppm out of range %x\n", __func__, (data_fine & 0x7ff));
        data_fine = AR_PHY_CHAN_INFO_GAIN_DIFF_UPPER_LIMIT;
    }
    /*
     * Restore signed value if changed above.
     * Use typecast to avoid compilation errors
     */
    if (signed_val)
        data_fine = (-(int32_t)data_fine) & AR_PHY_CHAN_INFO_GAIN_DIFF_PPM_MASK;

    /* write value */
    data4 = OS_REG_READ(ah, AR_PHY_TIMING2) & ~(AR_PHY_TIMING2_USE_FORCE_PPM | AR_PHY_TIMING2_FORCE_PPM_VAL);
    OS_REG_WRITE(ah, AR_PHY_TIMING2, data4 | data_fine | AR_PHY_TIMING2_USE_FORCE_PPM);

    return data_fine;
 }

void
ar9300PpmUnForce(struct ath_hal *ah)
{
    u_int32_t data4;

    data4 = OS_REG_READ(ah, AR_PHY_TIMING2) & ~AR_PHY_TIMING2_USE_FORCE_PPM;
    OS_REG_WRITE(ah, AR_PHY_TIMING2, data4);
}

u_int32_t
ar9300PpmArmTrigger(struct ath_hal *ah)
{
    u_int32_t val;
    u_int32_t ret;

    val = OS_REG_READ(ah, AR_PHY_CHAN_INFO_MEMORY);
    ret = OS_REG_READ(ah, AR_TSF_L32);
    OS_REG_WRITE(ah, AR_PHY_CHAN_INFO_MEMORY, val | AR_PHY_CHAN_INFO_MEMORY_CAPTURE_MASK);

    /* return low word of TSF at arm time */
    return ret;
}

int
ar9300PpmGetTrigger(struct ath_hal *ah)
{
    if (OS_REG_READ(ah, AR_PHY_CHAN_INFO_MEMORY) & AR_PHY_CHAN_INFO_MEMORY_CAPTURE_MASK) {
        /* has not triggered yet, return false */
        return 0;
    }

    /* else triggered, return true */
    return 1;
}

void
ar9300MarkPhyInactive(struct ath_hal *ah)
{
    OS_REG_WRITE(ah, AR_PHY_ACTIVE, AR_PHY_ACTIVE_DIS);
}

// DEBUG
u_int32_t
ar9300PpmGetForceState(struct ath_hal *ah)
{
    return (OS_REG_READ(ah, AR_PHY_TIMING2) & (AR_PHY_TIMING2_USE_FORCE_PPM | AR_PHY_TIMING2_FORCE_PPM_VAL));
}

#ifdef ATH_CCX
/*
 * Return the Cycle counts for rx_frame, rx_clear, and tx_frame
 */
void
ar9300GetMibCycleCounts(struct ath_hal *ah, HAL_COUNTERS* pCnts)
{

    pCnts->txFrameCount = OS_REG_READ(ah, AR_TFCNT);
    pCnts->rxFrameCount = OS_REG_READ(ah, AR_RFCNT);
    pCnts->rxClearCount = OS_REG_READ(ah, AR_RCCNT);
    pCnts->cycleCount   = OS_REG_READ(ah, AR_CCCNT);
    pCnts->isTxActive   = (OS_REG_READ(ah, AR_TFCNT) ==
                           pCnts->txFrameCount) ? AH_FALSE : AH_TRUE;
    pCnts->isRxActive   = (OS_REG_READ(ah, AR_RFCNT) ==
                           pCnts->rxFrameCount) ? AH_FALSE : AH_TRUE;
}

void
ar9300ClearMibCounters(struct ath_hal *ah)
{
    u_int32_t regVal;

    regVal = OS_REG_READ(ah, AR_MIBC);
    OS_REG_WRITE(ah, AR_MIBC, regVal | AR_MIBC_CMC);
    OS_REG_WRITE(ah, AR_MIBC, regVal & ~AR_MIBC_CMC);
}

u_int8_t
ar9300GetCcaThreshold(struct ath_hal *ah)
{
    return (u_int8_t)MS(OS_REG_READ(ah, AR_PHY_CCA_0), AR_PHY_CCA_THRESH62);
}
#endif

/* Enable or Disable RIFS Rx capability as part of SW WAR for Bug 31602 */
HAL_BOOL
ar9300SetRifsDelay(struct ath_hal *ah, HAL_BOOL enable)
{
    struct ath_hal_9300 *ahp = AH9300(ah);
    HAL_BOOL is_chan_2g = IS_CHAN_2GHZ(AH_PRIVATE(ah)->ah_curchan);
    u_int32_t tmp = 0;    

    if (enable) {
        if (ahp->ah_rifs_enabled == AH_TRUE)
            return AH_TRUE;

        OS_REG_WRITE(ah, AR_PHY_SEARCH_START_DELAY, ahp->ah_rifs_reg[0]);
        OS_REG_WRITE(ah, AR_PHY_RIFS_SRCH,
                     ahp->ah_rifs_reg[1]);

        ahp->ah_rifs_enabled = AH_TRUE;
        OS_MEMZERO(ahp->ah_rifs_reg, sizeof(ahp->ah_rifs_reg));
    } else {
        if (ahp->ah_rifs_enabled == AH_TRUE) {
            ahp->ah_rifs_reg[0] = OS_REG_READ(ah,
                                              AR_PHY_SEARCH_START_DELAY);
            ahp->ah_rifs_reg[1] = OS_REG_READ(ah, AR_PHY_RIFS_SRCH);
        }
        // Change rifs init delay to 0
        OS_REG_WRITE(ah, AR_PHY_RIFS_SRCH,
                     (ahp->ah_rifs_reg[1] & ~(AR_PHY_RIFS_INIT_DELAY)));
        tmp = 0xfffff000 & OS_REG_READ(ah,AR_PHY_SEARCH_START_DELAY);        
        if (is_chan_2g) {
            if (IS_CHAN_HT40(AH_PRIVATE(ah)->ah_curchan)) {
                OS_REG_WRITE(ah, AR_PHY_SEARCH_START_DELAY, tmp|0x268);
            } else {  // Sowl 2G HT-20 default is 0x134 for search start delay
                OS_REG_WRITE(ah, AR_PHY_SEARCH_START_DELAY, tmp|0x134);
            }
        } else {
            if (IS_CHAN_HT40(AH_PRIVATE(ah)->ah_curchan)) {
                OS_REG_WRITE(ah, AR_PHY_SEARCH_START_DELAY, tmp|0x370);
            } else { // Sowl 5G HT-20 default is 0x1b8 for search start delay
                OS_REG_WRITE(ah, AR_PHY_SEARCH_START_DELAY, tmp|0x1b8);
            }
        }

        ahp->ah_rifs_enabled = AH_FALSE;
    }
    return AH_TRUE;

} /* ar9300SetRifsDelay () */

/* Set the current RIFS Rx setting */
HAL_BOOL
ar9300Set11nRxRifs(struct ath_hal *ah, HAL_BOOL enable)
{
    /* Non-Owl 11n chips */
    if ((ath_hal_getcapability(ah, HAL_CAP_RIFS_RX, 0, AH_NULL) == HAL_OK)) {
        if (ar9300GetCapability(ah, HAL_CAP_LDPCWAR, 0, AH_NULL) == HAL_OK)
            {return ar9300SetRifsDelay(ah, enable);}
        return AH_FALSE;
    }

    return AH_TRUE;
} /* ar9300Set11nRxRifs () */

/* Get the current RIFS Rx setting */
HAL_BOOL
ar9300Get11nRxRifs(struct ath_hal *ah)
{
    struct ath_hal_9300 *ahp = AH9300(ah);

    if ((ath_hal_getcapability(ah, HAL_CAP_RIFS_RX, 0, AH_NULL) == HAL_OK))
        return ahp->ah_rifs_enabled;
    else
        return AH_FALSE;

} /* ar9300Get11nRxRifs () */

static hal_mac_hangs_t
ar9300CompareDbgHang(struct ath_hal *ah, mac_dbg_regs_t mac_dbg,
                     hal_mac_hang_check_t hang_check, hal_mac_hangs_t hangs)
{
    int i = 0;
    hal_mac_hangs_t found_hangs = 0;

    if (hangs & dcu_chain_state) {
        for (i = 0; i < 6; i++) {
            if (((mac_dbg.dma_dbg_4 >> (5*i)) & 0x1f) ==
                  hang_check.dcu_chain_state)
                found_hangs |= dcu_chain_state;
        }
        for (i = 0; i < 4; i++) {
            if (((mac_dbg.dma_dbg_5 >> (5*i)) & 0x1f) ==
                  hang_check.dcu_chain_state)
                found_hangs |= dcu_chain_state;
        }
    }

    if (hangs & dcu_complete_state) {
        if ((mac_dbg.dma_dbg_6 & 0x3) == hang_check.dcu_complete_state)
            found_hangs |= dcu_complete_state;
    }

    if (hangs & qcu_stitch_state) {
        if (((mac_dbg.dma_dbg_3 >> 18) & 0xf) == hang_check.qcu_stitch_state)
            found_hangs |= qcu_stitch_state;
    }

    if (hangs & qcu_fetch_state) {
        if (((mac_dbg.dma_dbg_3 >> 22) & 0xf) == hang_check.qcu_fetch_state)
            found_hangs |= qcu_fetch_state;
    }

    if (hangs & qcu_complete_state) {
        if (((mac_dbg.dma_dbg_3 >> 26) & 0x7) == hang_check.qcu_complete_state)
            found_hangs |= qcu_complete_state;
    }

    return found_hangs;

} /* end - ar9300CompareDbgHang */

#define NUM_STATUS_READS 50
HAL_BOOL
ar9300DetectMacHang(struct ath_hal *ah)
{
    struct ath_hal_9300 *ahp = AH9300(ah);
    mac_dbg_regs_t mac_dbg;
    hal_mac_hang_check_t hang_sig1_val = {0x6, 0x1, 0, 0, 0, 0, 0, 0},
                         hang_sig2_val = {0, 0, 0, 0, 0, 0x9, 0x8, 0x4};
    hal_mac_hangs_t      hang_sig1 = (dcu_chain_state | dcu_complete_state),
                         hang_sig2 = (qcu_stitch_state |
                                      qcu_fetch_state |
                                      qcu_complete_state);
    int i = 0;

    if (!(ahp->ah_hang_wars & HAL_MAC_HANG_WAR)) {
        return AH_FALSE;
    }

    OS_MEMZERO(&mac_dbg, sizeof(mac_dbg));

    mac_dbg.dma_dbg_3 = OS_REG_READ(ah, AR_DMADBG_3);
    mac_dbg.dma_dbg_4 = OS_REG_READ(ah, AR_DMADBG_4);
    mac_dbg.dma_dbg_5 = OS_REG_READ(ah, AR_DMADBG_5);
    mac_dbg.dma_dbg_6 = OS_REG_READ(ah, AR_DMADBG_6);

    for (i=1; i <= NUM_STATUS_READS; i++) {
        if (mac_dbg.dma_dbg_3 != OS_REG_READ(ah, AR_DMADBG_3) ||
            mac_dbg.dma_dbg_4 != OS_REG_READ(ah, AR_DMADBG_4) ||
            mac_dbg.dma_dbg_5 != OS_REG_READ(ah, AR_DMADBG_5) ||
            mac_dbg.dma_dbg_6 != OS_REG_READ(ah, AR_DMADBG_6))
            {
             return AH_FALSE;
            }
    }

    if (hang_sig1==ar9300CompareDbgHang(ah, mac_dbg, hang_sig1_val, hang_sig1)){
        HDPRINTF(ah, HAL_DBG_DFS, "%s sig5count=%d sig6count=%d ", __func__,
                 ahp->ah_hang[MAC_HANG_SIG1], ahp->ah_hang[MAC_HANG_SIG2]);
        ahp->ah_hang[MAC_HANG_SIG1]++;
        return AH_TRUE;
    }
    if (hang_sig2==ar9300CompareDbgHang(ah, mac_dbg, hang_sig2_val, hang_sig2)){
        HDPRINTF(ah, HAL_DBG_DFS, "%s sig5count=%d sig6count=%d ", __func__,
                 ahp->ah_hang[MAC_HANG_SIG1], ahp->ah_hang[MAC_HANG_SIG2]);
        ahp->ah_hang[MAC_HANG_SIG2]++;
        return AH_TRUE;
    }

    HDPRINTF(ah, HAL_DBG_DFS, "%s Found an unknown MAC hang signature "
                              "DMA Debug3=0x%x DMA Debug4=0x%x "
                              "DMA Debug5=0x%x DMA Debug6=0x%x\n", __func__,
             mac_dbg.dma_dbg_3, mac_dbg.dma_dbg_4, mac_dbg.dma_dbg_5,
             mac_dbg.dma_dbg_6);

    return AH_FALSE;

} /* end - ar9300DetectMacHang */

/* Determine if the baseband is hung by reading the Observation Bus Register */
HAL_BOOL
ar9300DetectBbHang(struct ath_hal *ah)
{
#define N(a) (sizeof(a)/sizeof(a[0]))
    struct ath_hal_9300 *ahp = AH9300(ah);
    u_int32_t hang_sig = 0;
    int i=0;
    /* Check the PCU Observation Bus 1 register (0x806c) NUM_STATUS_READS times
     *
     * 4 known BB hang signatures -
     * [1] bits 8,9,11 are 0. State machine state (bits 25-31) is 0x1E
     * [2] bits 8,9 are 1, bit 11 is 0. State machine state (bits 25-31) is 0x52
     * [3] bits 8,9 are 1, bit 11 is 0. State machine state (bits 25-31) is 0x18
     * [4] bit 10 is 1, bit 11 is 0. WEP state (bits 12-17) is 0x2,
     *     Rx State (bits 20-24) is 0x7.
     */
    hal_hw_hang_check_t hang_list [] =
    {
     /* Offset        Reg Value   Reg Mask    Hang Offset */
       {AR_OBS_BUS_1, 0x1E000000, 0x7E000B00, BB_HANG_SIG1},
       {AR_OBS_BUS_1, 0x52000B00, 0x7E000B00, BB_HANG_SIG2},
       {AR_OBS_BUS_1, 0x18000B00, 0x7E000B00, BB_HANG_SIG3},
       {AR_OBS_BUS_1, 0x00702400, 0x7E7FFFEF, BB_HANG_SIG4}
    };

    if (!(ahp->ah_hang_wars & (HAL_RIFS_BB_HANG_WAR |
                               HAL_DFS_BB_HANG_WAR |
                               HAL_RX_STUCK_LOW_BB_HANG_WAR))) {
         return AH_FALSE;
    }

    hang_sig = OS_REG_READ(ah, AR_OBS_BUS_1);
    for (i=1; i <= NUM_STATUS_READS; i++) {
        if (hang_sig != OS_REG_READ(ah, AR_OBS_BUS_1)) {
             return AH_FALSE;
        }
    }

    for (i=0; i < N(hang_list); i++) {
        if ((hang_sig & hang_list[i].hang_mask) == hang_list[i].hang_val) {
            ahp->ah_hang[hang_list[i].hang_offset]++;
            HDPRINTF(ah, HAL_DBG_DFS, "%s sig1count=%d sig2count=%d "
                     "sig3count=%d sig4count=%d\n", __func__,
                     ahp->ah_hang[BB_HANG_SIG1], ahp->ah_hang[BB_HANG_SIG2],
                     ahp->ah_hang[BB_HANG_SIG3], ahp->ah_hang[BB_HANG_SIG4]);
            return AH_TRUE;
        }
    }

    HDPRINTF(ah, HAL_DBG_DFS, "%s Found an unknown BB hang signature! "
                              "<0x806c>=0x%x\n", __func__, hang_sig);

    return AH_FALSE;

#undef N
} /* end - ar9300DetectBbHang () */

#undef NUM_STATUS_READS

HAL_STATUS
ar9300SelectAntConfig(struct ath_hal *ah, u_int32_t cfg)
{
    struct ath_hal_9300     *ahp = AH9300(ah);
    HAL_CHANNEL_INTERNAL    *chan = AH_PRIVATE(ah)->ah_curchan;
    const HAL_CAPABILITIES  *pCap = &AH_PRIVATE(ah)->ah_caps;
    u_int16_t               ant_config;
    u_int32_t               halNumAntConfig;

    halNumAntConfig = IS_CHAN_2GHZ(chan)? pCap->halNumAntCfg2GHz : pCap->halNumAntCfg5GHz;

    if (cfg < halNumAntConfig) {
        if (HAL_OK == ar9300EepromGetAntCfg(ahp, chan, cfg, &ant_config)) {
            OS_REG_WRITE(ah, AR_PHY_SWITCH_COM, ant_config);
            return HAL_OK;
        }
    }

    return HAL_EINVAL;
}

/* Get the number of antenna config ( Only checks the 2.4GHz) */
int ar9300GetNumAntCfg(struct ath_hal *ah)
{
    const HAL_CAPABILITIES  *pCap = &AH_PRIVATE(ah)->ah_caps;
    return pCap->halNumAntCfg2GHz;
}

/*
 * Functions to get/set DCS mode
 */
void
ar9300SetDcsMode(struct ath_hal *ah, u_int32_t mode)
{
    AH_PRIVATE(ah)->ah_dcs_enable = mode;
}

u_int32_t
ar9300GetDcsMode(struct ath_hal *ah)
{
    return AH_PRIVATE(ah)->ah_dcs_enable;
}

#ifdef ATH_BT_COEX
void
ar9300SetBTCoexInfo(struct ath_hal *ah, HAL_BT_COEX_INFO *btinfo)
{
    struct ath_hal_9300 *ahp = AH9300(ah);

    ahp->ah_btModule = btinfo->bt_module;
    ahp->ah_btCoexConfigType = btinfo->bt_coexConfig;
    ahp->ah_btActiveGpioSelect = btinfo->bt_gpioBTActive;
    ahp->ah_btPriorityGpioSelect = btinfo->bt_gpioBTPriority;
    ahp->ah_wlanActiveGpioSelect = btinfo->bt_gpioWLANActive;
    ahp->ah_btActivePolarity = btinfo->bt_activePolarity;
    ahp->ah_btCoexSingleAnt = btinfo->bt_singleAnt;
    ahp->ah_btWlanIsolation = btinfo->bt_isolation;
}

void
ar9300BTCoexConfig(struct ath_hal *ah, HAL_BT_COEX_CONFIG *btconf)
{
    struct ath_hal_9300 *ahp = AH9300(ah);
    HAL_BOOL rxClearPolarity = btconf->bt_rxclearPolarity;

    /*
     * For Kiwi and Osprey, the polarity of rx_clear is active high.
     * The bt_rxclearPolarity flag from ath_dev needs to be inverted.
     */
    rxClearPolarity = !btconf->bt_rxclearPolarity;

    ahp->ah_btCoexMode = (ahp->ah_btCoexMode & AR_BT_QCU_THRESH) |
        SM(btconf->bt_timeExtend, AR_BT_TIME_EXTEND) |
        SM(btconf->bt_txstateExtend, AR_BT_TXSTATE_EXTEND) |
        SM(btconf->bt_txframeExtend, AR_BT_TX_FRAME_EXTEND) |
        SM(btconf->bt_mode, AR_BT_MODE) |
        SM(btconf->bt_quietCollision, AR_BT_QUIET) |
        SM(rxClearPolarity, AR_BT_RX_CLEAR_POLARITY) |
        SM(btconf->bt_priorityTime, AR_BT_PRIORITY_TIME) |
        SM(btconf->bt_firstSlotTime, AR_BT_FIRST_SLOT_TIME);

    ahp->ah_btCoexMode2 |= SM(btconf->bt_holdRxclear, AR_BT_HOLD_RX_CLEAR);

    if (ahp->ah_btCoexSingleAnt == AH_FALSE) {
        /* Enable ACK to go out even though BT has higher priority. */
        ahp->ah_btCoexMode2 |= AR_BT_DISABLE_BT_ANT;
    }
}

void
ar9300BTCoexSetQcuThresh(struct ath_hal *ah, int qnum)
{
    struct ath_hal_9300 *ahp = AH9300(ah);
    ahp->ah_btCoexMode |= SM(qnum, AR_BT_QCU_THRESH);
}

void
ar9300BTCoexSetWeights(struct ath_hal *ah, u_int32_t stompType)
{
    struct ath_hal_9300 *ahp = AH9300(ah);

    ahp->ah_btCoexBTWeight[0] = AR9300_BT_WGHT;
    ahp->ah_btCoexBTWeight[1] = AR9300_BT_WGHT;
    ahp->ah_btCoexBTWeight[2] = AR9300_BT_WGHT;
    ahp->ah_btCoexBTWeight[3] = AR9300_BT_WGHT;

    switch(stompType) {
    case HAL_BT_COEX_STOMP_ALL:
        ahp->ah_btCoexWLANWeight[0] = AR9300_STOMP_ALL_WLAN_WGHT0;
        ahp->ah_btCoexWLANWeight[1] = AR9300_STOMP_ALL_WLAN_WGHT1;
        break;
    case HAL_BT_COEX_STOMP_LOW:
        ahp->ah_btCoexWLANWeight[0] = AR9300_STOMP_LOW_WLAN_WGHT0;
        ahp->ah_btCoexWLANWeight[1] = AR9300_STOMP_LOW_WLAN_WGHT1;
        break;
    case HAL_BT_COEX_STOMP_ALL_FORCE:
        ahp->ah_btCoexWLANWeight[0] = AR9300_STOMP_ALL_FORCE_WLAN_WGHT0;
        ahp->ah_btCoexWLANWeight[1] = AR9300_STOMP_ALL_FORCE_WLAN_WGHT1;
        break;
    case HAL_BT_COEX_STOMP_LOW_FORCE:
        ahp->ah_btCoexWLANWeight[0] = AR9300_STOMP_LOW_FORCE_WLAN_WGHT0;
        ahp->ah_btCoexWLANWeight[1] = AR9300_STOMP_LOW_FORCE_WLAN_WGHT1;
        break;
    case HAL_BT_COEX_STOMP_NONE:
    case HAL_BT_COEX_NO_STOMP:
        ahp->ah_btCoexWLANWeight[0] = AR9300_STOMP_NONE_WLAN_WGHT0;
        ahp->ah_btCoexWLANWeight[1] = AR9300_STOMP_NONE_WLAN_WGHT1;
        break;
    default:
        /* There is a forceWeight from registry */
        ahp->ah_btCoexWLANWeight[0] = stompType;
        ahp->ah_btCoexWLANWeight[1] = stompType;
        break;
    }
}

void
ar9300BTCoexSetupBmissThresh(struct ath_hal *ah, u_int32_t thresh)
{
    struct ath_hal_9300 *ahp = AH9300(ah);
    ahp->ah_btCoexMode2 |= SM(thresh, AR_BT_BCN_MISS_THRESH);
}

void
ar9300BTCoexSetParameter(struct ath_hal *ah, u_int32_t type, u_int32_t value)
{
    struct ath_hal_9300 *ahp = AH9300(ah);

    switch (type) {
        case HAL_BT_COEX_SET_ACK_PWR:
            if (value) {
                ahp->ah_btCoexFlag |= HAL_BT_COEX_FLAG_LOW_ACK_PWR;
                OS_REG_WRITE(ah, AR_TPC, HAL_BT_COEX_LOW_ACK_POWER);
            }
            else {
                ahp->ah_btCoexFlag &= ~HAL_BT_COEX_FLAG_LOW_ACK_PWR;
                OS_REG_WRITE(ah, AR_TPC, HAL_BT_COEX_HIGH_ACK_POWER);
            }
            break;

        default:
            break;
    }
}

void
ar9300BTCoexDisable(struct ath_hal *ah)
{
    struct ath_hal_9300 *ahp = AH9300(ah);

    /* Always drive rx_clear_external output as 0 */
    ath_hal_gpioCfgOutput(ah, ahp->ah_wlanActiveGpioSelect, HAL_GPIO_OUTPUT_MUX_AS_OUTPUT);

    if (ahp->ah_btCoexSingleAnt == AH_TRUE) {
        OS_REG_RMW_FIELD(ah, AR_QUIET1, AR_QUIET1_QUIET_ACK_CTS_ENABLE, 1); 
        OS_REG_RMW_FIELD(ah, AR_PCU_MISC, AR_PCU_BT_ANT_PREVENT_RX, 0);
    }

    OS_REG_WRITE(ah, AR_BT_COEX_MODE, AR_BT_QUIET | AR_BT_MODE);
    OS_REG_WRITE(ah, AR_BT_COEX_MODE2, 0);
    OS_REG_WRITE(ah, AR_BT_COEX_WL_WEIGHTS0, 0);
    OS_REG_WRITE(ah, AR_BT_COEX_WL_WEIGHTS1, 0);
    OS_REG_WRITE(ah, AR_BT_COEX_BT_WEIGHTS0, 0);
    OS_REG_WRITE(ah, AR_BT_COEX_BT_WEIGHTS1, 0);
    OS_REG_WRITE(ah, AR_BT_COEX_BT_WEIGHTS2, 0);
    OS_REG_WRITE(ah, AR_BT_COEX_BT_WEIGHTS3, 0);

    ahp->ah_btCoexEnabled = AH_FALSE;
}

int
ar9300BTCoexEnable(struct ath_hal *ah)
{
    struct ath_hal_9300 *ahp = AH9300(ah);

    /* Program coex mode and weight registers to actually enable coex */
    OS_REG_WRITE(ah, AR_BT_COEX_MODE, ahp->ah_btCoexMode);
    OS_REG_WRITE(ah, AR_BT_COEX_MODE2, ahp->ah_btCoexMode2);
    OS_REG_WRITE(ah, AR_BT_COEX_WL_WEIGHTS0, ahp->ah_btCoexWLANWeight[0]);
    OS_REG_WRITE(ah, AR_BT_COEX_WL_WEIGHTS1, ahp->ah_btCoexWLANWeight[1]);
    OS_REG_WRITE(ah, AR_BT_COEX_BT_WEIGHTS0, ahp->ah_btCoexBTWeight[0]);
    OS_REG_WRITE(ah, AR_BT_COEX_BT_WEIGHTS1, ahp->ah_btCoexBTWeight[1]);
    OS_REG_WRITE(ah, AR_BT_COEX_BT_WEIGHTS2, ahp->ah_btCoexBTWeight[2]);
    OS_REG_WRITE(ah, AR_BT_COEX_BT_WEIGHTS3, ahp->ah_btCoexBTWeight[3]);

    if (ahp->ah_btCoexFlag & HAL_BT_COEX_FLAG_LOW_ACK_PWR) {
        OS_REG_WRITE(ah, AR_TPC, HAL_BT_COEX_LOW_ACK_POWER);
    }
    else {
        OS_REG_WRITE(ah, AR_TPC, HAL_BT_COEX_HIGH_ACK_POWER);
    }

    if (ahp->ah_btCoexSingleAnt == AH_TRUE) {
        OS_REG_RMW_FIELD(ah, AR_QUIET1, AR_QUIET1_QUIET_ACK_CTS_ENABLE, 1); 
        OS_REG_RMW_FIELD(ah, AR_PCU_MISC, AR_PCU_BT_ANT_PREVENT_RX, 1);
    }
    else {
        OS_REG_RMW_FIELD(ah, AR_QUIET1, AR_QUIET1_QUIET_ACK_CTS_ENABLE, 1); 
        OS_REG_RMW_FIELD(ah, AR_PCU_MISC, AR_PCU_BT_ANT_PREVENT_RX, 0);
    }

    if (ahp->ah_btCoexConfigType == HAL_BT_COEX_CFG_3WIRE) {
        /* For 3-wire, configure the desired GPIO port for rx_clear */
        ath_hal_gpioCfgOutput(ah, ahp->ah_wlanActiveGpioSelect, HAL_GPIO_OUTPUT_MUX_AS_WLAN_ACTIVE);
    }
    else {
        /* For 2-wire, configure the desired GPIO port for TX_FRAME output */
        ath_hal_gpioCfgOutput(ah, ahp->ah_wlanActiveGpioSelect, HAL_GPIO_OUTPUT_MUX_AS_TX_FRAME);
    }

    /* enable a weak pull down on BT_ACTIVE. When BT device is disabled, BT_ACTIVE might be floating. */
    OS_REG_RMW(ah, AR_HOSTIF_REG(ah, AR_GPIO_PDPU), (AR_GPIO_PULL_DOWN << (ahp->ah_btActiveGpioSelect * 2)), 
              (AR_GPIO_PDPU_OPTION << (ahp->ah_btActiveGpioSelect * 2)));

    ahp->ah_btCoexEnabled = AH_TRUE;

    return 0;
}

void
ar9300InitBTCoex(struct ath_hal *ah)
{
    struct ath_hal_9300 *ahp = AH9300(ah);

    if (ahp->ah_btCoexConfigType == HAL_BT_COEX_CFG_3WIRE) {
        OS_REG_SET_BIT(ah, AR_HOSTIF_REG(ah, AR_GPIO_INPUT_EN_VAL),
                   (AR_GPIO_INPUT_EN_VAL_BT_PRIORITY_BB |
                    AR_GPIO_INPUT_EN_VAL_BT_ACTIVE_BB));

        /* Set input mux for bt_prority_async and bt_active_async to GPIO pins */
        OS_REG_RMW_FIELD(ah, AR_HOSTIF_REG(ah, AR_GPIO_INPUT_MUX1), AR_GPIO_INPUT_MUX1_BT_ACTIVE,
                     ahp->ah_btActiveGpioSelect);
        OS_REG_RMW_FIELD(ah, AR_HOSTIF_REG(ah, AR_GPIO_INPUT_MUX1), AR_GPIO_INPUT_MUX1_BT_PRIORITY,
                         ahp->ah_btPriorityGpioSelect);

        /* Configure the desired GPIO ports for input */
        ath_hal_gpioCfgInput(ah, ahp->ah_btActiveGpioSelect);
        ath_hal_gpioCfgInput(ah, ahp->ah_btPriorityGpioSelect); 

        if (ahp->ah_btCoexEnabled) {
            ar9300BTCoexEnable(ah);
        }else {
            ar9300BTCoexDisable(ah);
        }
    }
    else if (ahp->ah_btCoexConfigType != HAL_BT_COEX_CFG_NONE) {
        /* 2-wire */
        if (ahp->ah_btCoexEnabled) {
            /* Connect bt_active_async to baseband */
            OS_REG_CLR_BIT(ah, AR_HOSTIF_REG(ah, AR_GPIO_INPUT_EN_VAL), 
                            (AR_GPIO_INPUT_EN_VAL_BT_PRIORITY_DEF | AR_GPIO_INPUT_EN_VAL_BT_FREQUENCY_DEF));
            OS_REG_SET_BIT(ah, AR_HOSTIF_REG(ah, AR_GPIO_INPUT_EN_VAL),
                            AR_GPIO_INPUT_EN_VAL_BT_ACTIVE_BB);

            /* Set input mux for bt_prority_async and bt_active_async to GPIO pins */
            OS_REG_RMW_FIELD(ah, AR_HOSTIF_REG(ah, AR_GPIO_INPUT_MUX1), AR_GPIO_INPUT_MUX1_BT_ACTIVE,
                             ahp->ah_btActiveGpioSelect);

            /* Configure the desired GPIO ports for input */
            ath_hal_gpioCfgInput(ah, ahp->ah_btActiveGpioSelect);

            /* Enable coexistence on initialization */
            ar9300BTCoexEnable(ah);
        }
    }
}

#endif /* ATH_BT_COEX */
HAL_STATUS ar9300SetProxySTA(struct ath_hal *ah, HAL_BOOL enable)
{
    HAL_STATUS retval = HAL_OK;
    if(enable){
         OS_REG_WRITE(ah, AR_AZIMUTH_MODE, OS_REG_READ(ah, AR_AZIMUTH_MODE) | AR_AZIMUTH_KEY_SEARCH_AD1 | AR_AZIMUTH_CTS_MATCH_TX_AD2 | AR_AZIMUTH_BA_USES_AD1);
     }else{
         OS_REG_WRITE(ah, AR_AZIMUTH_MODE, OS_REG_READ(ah, AR_AZIMUTH_MODE) & ~( AR_AZIMUTH_KEY_SEARCH_AD1 | AR_AZIMUTH_CTS_MATCH_TX_AD2 | AR_AZIMUTH_BA_USES_AD1));
     }
    //ath_hal_printf(ah, "%s[%d] reg 0x%04x = 0x%08x\n", __func__, __LINE__, AR_AZIMUTH_MODE, OS_REG_READ(ah, AR_AZIMUTH_MODE));
    return retval;
}

void ar9300EnableTPC(struct ath_hal *ah)
{
    AH_PRIVATE(ah)->ah_config.ath_hal_desc_tpc = 1;
    OS_REG_WRITE(ah, AR_PHY_POWER_TX_RATE_MAX, MAX_RATE_POWER |AR_PHY_POWER_TX_RATE_MAX_TPC_ENABLE);
}


/*
 * ar9300ForceTSFSync 
 *      This function forces the TSF sync to the given bssid, this is implemented
 *      as a temp hack to get the AoW demo, and is primarily used in the WDS client
 *      mode of operation, where we sync the TSF to RootAP TSF values
 */
void
ar9300ForceTSFSync(struct ath_hal *ah, const u_int8_t *bssid, u_int16_t assocId)
{
    ar9300SetOperatingMode(ah, HAL_M_STA);
    ar9300WriteAssocid(ah, bssid, assocId);
}

void ar9300ChkRSSIUpdateTxPwr(struct ath_hal *ah, int rssi)
{
    struct ath_hal_9300 *ahp = AH9300(ah);
    u_int32_t           temp_obdb_reg_val = 0, temp_tcp_reg_val;
    u_int32_t           temp_powertx_rate9_reg_val;
    int8_t              olpc_power_offset = 0;
    int8_t              tmp_olpc_val = 0;
    HAL_RSSI_TX_POWER   old_greentx_status;
    u_int8_t            target_power_val_t[ar9300RateSize];
    int8_t              tmp_rss1_thr1, tmp_rss1_thr2;

    if ((AH_PRIVATE(ah)->ah_opmode != HAL_M_STA) || 
        !AH_PRIVATE(ah)->ah_config.ath_hal_sta_update_tx_pwr_enable)
    {
        return;
    }
    
    old_greentx_status = ah->green_tx_status;
    if (ahp->ah_hw_green_tx_enable) {
        tmp_rss1_thr1 = AR9485_HW_GREEN_TX_THRES1_DB;
        tmp_rss1_thr2 = AR9485_HW_GREEN_TX_THRES2_DB;
    } else {
        tmp_rss1_thr1 = WB225_SW_GREEN_TX_THRES1_DB;
        tmp_rss1_thr2 = WB225_SW_GREEN_TX_THRES2_DB;
    }
    
    if ((AH_PRIVATE(ah)->ah_config.ath_hal_sta_update_tx_pwr_enable_S1) 
        && (rssi > tmp_rss1_thr1)) 
    {
        if (old_greentx_status != HAL_RSSI_TX_POWER_SHORT) {
            ah->green_tx_status = HAL_RSSI_TX_POWER_SHORT;
        }
    } else if (AH_PRIVATE(ah)->ah_config.ath_hal_sta_update_tx_pwr_enable_S2 
        && (rssi > tmp_rss1_thr2)) 
    {
        if (old_greentx_status != HAL_RSSI_TX_POWER_MIDDLE) {
            ah->green_tx_status = HAL_RSSI_TX_POWER_MIDDLE;
        }
    } else if (AH_PRIVATE(ah)->ah_config.ath_hal_sta_update_tx_pwr_enable_S3) {
        if (old_greentx_status != HAL_RSSI_TX_POWER_LONG) {
            ah->green_tx_status = HAL_RSSI_TX_POWER_LONG;
        }
    }

    /* If status is not change, don't do anything */
    if (old_greentx_status == ah->green_tx_status) {
        return;
    }
    
    /* for Poseidon which ath_hal_sta_update_tx_pwr_enable is enabled */
    if ((ah->green_tx_status != HAL_RSSI_TX_POWER_NONE) 
        && AR_SREV_POSEIDON(ah))
    {
        if (ahp->ah_hw_green_tx_enable) {
            switch (ah->green_tx_status) {
            case HAL_RSSI_TX_POWER_SHORT:
                /* 1. TxPower Config */
                OS_MEMCPY(target_power_val_t, ar9485_hw_gtx_tp_distance_short,
                    sizeof(target_power_val_t));
                /* 1.1 Store OLPC Delta Calibration Offset*/
                olpc_power_offset = 0;
                /* 2. Store OB/DB */
                /* 3. Store TPC settting */
                temp_tcp_reg_val = (SM(14, AR_TPC_ACK) |
                                    SM(14, AR_TPC_CTS) |
                                    SM(14, AR_TPC_CHIRP) |
                                    SM(14, AR_TPC_RPT));
                /* 4. Store BB_powertx_rate9 value */
                temp_powertx_rate9_reg_val = 
                    AR9485_BBPWRTXRATE9_HW_GREEN_TX_SHORT_VALUE;
                break;
            case HAL_RSSI_TX_POWER_MIDDLE:
                /* 1. TxPower Config */
                OS_MEMCPY(target_power_val_t, ar9485_hw_gtx_tp_distance_middle,
                sizeof(target_power_val_t));
                /* 1.1 Store OLPC Delta Calibration Offset*/
                olpc_power_offset = 0;
                /* 2. Store OB/DB */
                /* 3. Store TPC settting */
                temp_tcp_reg_val = (SM(18, AR_TPC_ACK) |
                                    SM(18, AR_TPC_CTS) |
                                    SM(18, AR_TPC_CHIRP) |
                                    SM(18, AR_TPC_RPT));
                /* 4. Store BB_powertx_rate9 value */
                temp_powertx_rate9_reg_val = 
                    AR9485_BBPWRTXRATE9_HW_GREEN_TX_MIDDLE_VALUE;
                break;
            case HAL_RSSI_TX_POWER_LONG:
            default:
                /* 1. TxPower Config */
                OS_MEMCPY(target_power_val_t, ahp->ah_default_tx_power,
                    sizeof(target_power_val_t));
                /* 1.1 Store OLPC Delta Calibration Offset*/
                olpc_power_offset = 0;
                /* 2. Store OB/DB1/DB2 */
                /* 3. Store TPC settting */
                temp_tcp_reg_val = ah->OBDB1[POSEIDON_STORED_REG_TPC];
                /* 4. Store BB_powertx_rate9 value */
                temp_powertx_rate9_reg_val = 
                    ah->OBDB1[POSEIDON_STORED_REG_BB_PWRTX_RATE9];
                break;
            }
        } else {
            switch (ah->green_tx_status) {
            case HAL_RSSI_TX_POWER_SHORT:
                /* 1. TxPower Config */
                OS_MEMCPY(target_power_val_t, wb225_sw_gtx_tp_distance_short,
                    sizeof(target_power_val_t));
                /* 1.1 Store OLPC Delta Calibration Offset*/
                olpc_power_offset = 
                    wb225_gtx_olpc_cal_offset[WB225_OB_GREEN_TX_SHORT_VALUE] -
                    wb225_gtx_olpc_cal_offset[WB225_OB_CALIBRATION_VALUE];
                /* 2. Store OB/DB */
                temp_obdb_reg_val = ah->OBDB1[POSEIDON_STORED_REG_OBDB];
                temp_obdb_reg_val &= ~(AR_PHY_65NM_CH0_TXRF2_DB2G | 
                                       AR_PHY_65NM_CH0_TXRF2_OB2G_CCK |
                                       AR_PHY_65NM_CH0_TXRF2_OB2G_PSK |
                                       AR_PHY_65NM_CH0_TXRF2_OB2G_QAM);
                temp_obdb_reg_val |= (SM(5, AR_PHY_65NM_CH0_TXRF2_DB2G) |
                SM(WB225_OB_GREEN_TX_SHORT_VALUE, 
                    AR_PHY_65NM_CH0_TXRF2_OB2G_CCK) |
                SM(WB225_OB_GREEN_TX_SHORT_VALUE, 
                    AR_PHY_65NM_CH0_TXRF2_OB2G_PSK) |
                SM(WB225_OB_GREEN_TX_SHORT_VALUE, 
                    AR_PHY_65NM_CH0_TXRF2_OB2G_QAM));
                /* 3. Store TPC settting */
                temp_tcp_reg_val = (SM(6, AR_TPC_ACK) |
                                    SM(6, AR_TPC_CTS) |
                                    SM(6, AR_TPC_CHIRP) |
                                    SM(6, AR_TPC_RPT));
                /* 4. Store BB_powertx_rate9 value */
                temp_powertx_rate9_reg_val = 
                    WB225_BBPWRTXRATE9_SW_GREEN_TX_SHORT_VALUE;
                break;
            case HAL_RSSI_TX_POWER_MIDDLE:
                /* 1. TxPower Config */
                OS_MEMCPY(target_power_val_t, wb225_sw_gtx_tp_distance_middle,
                    sizeof(target_power_val_t));
                /* 1.1 Store OLPC Delta Calibration Offset*/
                olpc_power_offset = 
                    wb225_gtx_olpc_cal_offset[WB225_OB_GREEN_TX_MIDDLE_VALUE] -
                    wb225_gtx_olpc_cal_offset[WB225_OB_CALIBRATION_VALUE];
                /* 2. Store OB/DB */
                temp_obdb_reg_val = ah->OBDB1[POSEIDON_STORED_REG_OBDB];
                temp_obdb_reg_val &= ~(AR_PHY_65NM_CH0_TXRF2_DB2G | 
                                       AR_PHY_65NM_CH0_TXRF2_OB2G_CCK |
                                       AR_PHY_65NM_CH0_TXRF2_OB2G_PSK |
                                       AR_PHY_65NM_CH0_TXRF2_OB2G_QAM);
                temp_obdb_reg_val |= (SM(5, AR_PHY_65NM_CH0_TXRF2_DB2G) |
                SM(WB225_OB_GREEN_TX_MIDDLE_VALUE, 
                    AR_PHY_65NM_CH0_TXRF2_OB2G_CCK) |
                SM(WB225_OB_GREEN_TX_MIDDLE_VALUE, 
                    AR_PHY_65NM_CH0_TXRF2_OB2G_PSK) |
                SM(WB225_OB_GREEN_TX_MIDDLE_VALUE, 
                    AR_PHY_65NM_CH0_TXRF2_OB2G_QAM));
                /* 3. Store TPC settting */
                temp_tcp_reg_val = (SM(14, AR_TPC_ACK) |
                                    SM(14, AR_TPC_CTS) |
                                    SM(14, AR_TPC_CHIRP) |
                                    SM(14, AR_TPC_RPT));
                /* 4. Store BB_powertx_rate9 value */
                temp_powertx_rate9_reg_val = 
                    WB225_BBPWRTXRATE9_SW_GREEN_TX_MIDDLE_VALUE;
                break;
            case HAL_RSSI_TX_POWER_LONG:
            default:
                /* 1. TxPower Config */
                OS_MEMCPY(target_power_val_t, ahp->ah_default_tx_power,
                    sizeof(target_power_val_t));
                /* 1.1 Store OLPC Delta Calibration Offset*/
                olpc_power_offset = 
                    wb225_gtx_olpc_cal_offset[WB225_OB_GREEN_TX_LONG_VALUE] -
                    wb225_gtx_olpc_cal_offset[WB225_OB_CALIBRATION_VALUE];
                /* 2. Store OB/DB1/DB2 */
                temp_obdb_reg_val = ah->OBDB1[POSEIDON_STORED_REG_OBDB];
                /* 3. Store TPC settting */
                temp_tcp_reg_val = ah->OBDB1[POSEIDON_STORED_REG_TPC];
                /* 4. Store BB_powertx_rate9 value */
                temp_powertx_rate9_reg_val = 
                    ah->OBDB1[POSEIDON_STORED_REG_BB_PWRTX_RATE9];
                break;
            }
        }
       
        /* 1.1 Do OLPC Delta Calibration Offset*/
        tmp_olpc_val = (int8_t) ah->DB2[POSEIDON_STORED_REG_G2_OLPC_OFFSET];
        tmp_olpc_val += olpc_power_offset;
        OS_REG_RMW(ah, AR_PHY_TPC_11_B0, 
            (tmp_olpc_val << AR_PHY_TPC_OLPC_GAIN_DELTA_S), 
            AR_PHY_TPC_OLPC_GAIN_DELTA);
 
        /* 1.2 TxPower Config */
        ar9300TransmitPowerRegWrite(ah, target_power_val_t);     
        /* 2. Config OB/DB */
        if (!ahp->ah_hw_green_tx_enable) {
            OS_REG_WRITE(ah, AR_PHY_65NM_CH0_TXRF2, temp_obdb_reg_val);
        }
        /* 3. config TPC settting */
        OS_REG_WRITE(ah, AR_TPC, temp_tcp_reg_val);
        /* 4. config BB_powertx_rate9 value */
        OS_REG_WRITE(ah, AR_PHY_BB_POWERTX_RATE9, temp_powertx_rate9_reg_val);
    }
}

/*
 * ar9300_is_skip_paprd_by_greentx
 *
 * This function check if we need to skip PAPRD tuning 
 * when GreenTx in specific state.
 */
HAL_BOOL 
ar9300_is_skip_paprd_by_greentx(struct ath_hal *ah)
{
    if (AR_SREV_POSEIDON(ah) && 
        AH_PRIVATE(ah)->ah_config.ath_hal_sta_update_tx_pwr_enable &&
        ((ah->green_tx_status == HAL_RSSI_TX_POWER_SHORT) || 
         (ah->green_tx_status == HAL_RSSI_TX_POWER_MIDDLE))) 
    {
        return AH_TRUE;
    }

    return AH_FALSE;
}
void
ar9300_get_vow_stats(struct ath_hal *ah, HAL_VOWSTATS* p_stats)
{
    p_stats->tx_frame_count = OS_REG_READ(ah, AR_TFCNT);
    p_stats->rx_frame_count = OS_REG_READ(ah, AR_RFCNT);
    p_stats->rx_clear_count = OS_REG_READ(ah, AR_RCCNT);
    p_stats->cycle_count   = OS_REG_READ(ah, AR_CCCNT);
    p_stats->ext_cycle_count   = OS_REG_READ(ah, AR_EXTRCCNT);
}

void
ar9300_control_signals_for_green_tx_mode(struct ath_hal *ah)
{
    unsigned int valid_obdb_0_b0 = 0x2d; // 5,5 - dB[0:2],oB[5:3]  
    unsigned int valid_obdb_1_b0 = 0x25; // 4,5 - dB[0:2],oB[5:3]  
    unsigned int valid_obdb_2_b0 = 0x1d; // 3,5 - dB[0:2],oB[5:3] 
    unsigned int valid_obdb_3_b0 = 0x15; // 2,5 - dB[0:2],oB[5:3] 
    unsigned int valid_obdb_4_b0 = 0xd;  // 1,5 - dB[0:2],oB[5:3]
    struct ath_hal_9300 *ahp = AH9300(ah);

	if (AR_SREV_POSEIDON(ah) && ahp->ah_hw_green_tx_enable) {
        OS_REG_RMW_FIELD_ALT(ah, AR_PHY_PAPRD_VALID_OBDB_POSEIDON, 
		    AR_PHY_PAPRD_VALID_OBDB_0, valid_obdb_0_b0);
        OS_REG_RMW_FIELD_ALT(ah, AR_PHY_PAPRD_VALID_OBDB_POSEIDON, 
		    AR_PHY_PAPRD_VALID_OBDB_1, valid_obdb_1_b0);	
        OS_REG_RMW_FIELD_ALT(ah, AR_PHY_PAPRD_VALID_OBDB_POSEIDON, 
		    AR_PHY_PAPRD_VALID_OBDB_2, valid_obdb_2_b0);	
        OS_REG_RMW_FIELD_ALT(ah, AR_PHY_PAPRD_VALID_OBDB_POSEIDON, 
		    AR_PHY_PAPRD_VALID_OBDB_3, valid_obdb_3_b0);	
        OS_REG_RMW_FIELD_ALT(ah, AR_PHY_PAPRD_VALID_OBDB_POSEIDON, 
		    AR_PHY_PAPRD_VALID_OBDB_4, valid_obdb_4_b0);	
	}
}

void ar9300_hwgreentx_set_pal_spare(struct ath_hal *ah, int value)
{
    struct ath_hal_9300 *ahp = AH9300(ah);

    if (AR_SREV_POSEIDON(ah) && ahp->ah_hw_green_tx_enable) {
        if ((value == 0) || (value == 1)) {
            OS_REG_RMW_FIELD(ah, AR_PHY_65NM_CH0_TXRF3, 
			    AR_PHY_65NM_CH0_TXRF3_OLD_PAL_SPARE, value);
	    }
	}
}
#endif /* AH_SUPPORT_AR9300 */
