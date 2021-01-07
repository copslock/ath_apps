/* 
 * ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is the elliptic curve math library for prime field curves.
 *
 * The Initial Developer of the Original Code is
 * Sun Microsystems, Inc.
 * Portions created by the Initial Developer are Copyright (C) 2003
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *   Sheueling Chang-Shantz <sheueling.chang@sun.com>,
 *   Stephen Fung <fungstep@hotmail.com>, and
 *   Douglas Stebila <douglas@stebila.ca>, Sun Microsystems Laboratories.
 *   Bodo Moeller <moeller@cdc.informatik.tu-darmstadt.de>,
 *   Nils Larsch <nla@trustcenter.de>, and
 *   Lenka Fibikova <fibikova@exp-math.uni-essen.de>, the OpenSSL Project
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */

#include "ecp.h"
#include "mplogic.h"
#include <stdlib.h>

/* Checks if point P(px, py) is at infinity.  Uses affine coordinates. */
mp_err
ec_GFp_pt_is_inf_aff(const mp_int *px, const mp_int *py)
{

	if ((mp_cmp_z(px) == 0) && (mp_cmp_z(py) == 0)) {
		return MP_YES;
	} else {
		return MP_NO;
	}

}

/* Sets P(px, py) to be the point at infinity.  Uses affine coordinates. */
mp_err
ec_GFp_pt_set_inf_aff(mp_int *px, mp_int *py)
{
	mp_zero(px);
	mp_zero(py);
	return MP_OKAY;
}

/* Computes R = P + Q based on IEEE P1363 A.10.1. Elliptic curve points P, 
 * Q, and R can all be identical. Uses affine coordinates. Assumes input
 * is already field-encoded using field_enc, and returns output that is
 * still field-encoded. */
mp_err
ec_GFp_pt_add_aff(const mp_int *px, const mp_int *py, const mp_int *qx,
				  const mp_int *qy, mp_int *rx, mp_int *ry,
				  const ECGroup *group)
{
	mp_err res = MP_OKAY;
	mp_int lambda, temp, tempx, tempy;

	MP_DIGITS(&lambda) = 0;
	MP_DIGITS(&temp) = 0;
	MP_DIGITS(&tempx) = 0;
	MP_DIGITS(&tempy) = 0;
	MP_CHECKOK(mp_init(&lambda));
	MP_CHECKOK(mp_init(&temp));
	MP_CHECKOK(mp_init(&tempx));
	MP_CHECKOK(mp_init(&tempy));
	/* if P = inf, then R = Q */
	if (ec_GFp_pt_is_inf_aff(px, py) == 0) {
		MP_CHECKOK(mp_copy(qx, rx));
		MP_CHECKOK(mp_copy(qy, ry));
		res = MP_OKAY;
		goto CLEANUP;
	}
	/* if Q = inf, then R = P */
	if (ec_GFp_pt_is_inf_aff(qx, qy) == 0) {
		MP_CHECKOK(mp_copy(px, rx));
		MP_CHECKOK(mp_copy(py, ry));
		res = MP_OKAY;
		goto CLEANUP;
	}
	/* if px != qx, then lambda = (py-qy) / (px-qx) */
	if (mp_cmp(px, qx) != 0) {
		MP_CHECKOK(group->meth->field_sub(py, qy, &tempy, group->meth));
		MP_CHECKOK(group->meth->field_sub(px, qx, &tempx, group->meth));
		MP_CHECKOK(group->meth->
				   field_div(&tempy, &tempx, &lambda, group->meth));
	} else {
		/* if py != qy or qy = 0, then R = inf */
		if (((mp_cmp(py, qy) != 0)) || (mp_cmp_z(qy) == 0)) {
			mp_zero(rx);
			mp_zero(ry);
			res = MP_OKAY;
			goto CLEANUP;
		}
		/* lambda = (3qx^2+a) / (2qy) */
		MP_CHECKOK(group->meth->field_sqr(qx, &tempx, group->meth));
		MP_CHECKOK(mp_set_int(&temp, 3));
		if (group->meth->field_enc) {
			MP_CHECKOK(group->meth->field_enc(&temp, &temp, group->meth));
		}
		MP_CHECKOK(group->meth->
				   field_mul(&tempx, &temp, &tempx, group->meth));
		MP_CHECKOK(group->meth->
				   field_add(&tempx, &group->curvea, &tempx, group->meth));
		MP_CHECKOK(mp_set_int(&temp, 2));
		if (group->meth->field_enc) {
			MP_CHECKOK(group->meth->field_enc(&temp, &temp, group->meth));
		}
		MP_CHECKOK(group->meth->field_mul(qy, &temp, &tempy, group->meth));
		MP_CHECKOK(group->meth->
				   field_div(&tempx, &tempy, &lambda, group->meth));
	}
	/* rx = lambda^2 - px - qx */
	MP_CHECKOK(group->meth->field_sqr(&lambda, &tempx, group->meth));
	MP_CHECKOK(group->meth->field_sub(&tempx, px, &tempx, group->meth));
	MP_CHECKOK(group->meth->field_sub(&tempx, qx, &tempx, group->meth));
	/* ry = (x1-x2) * lambda - y1 */
	MP_CHECKOK(group->meth->field_sub(qx, &tempx, &tempy, group->meth));
	MP_CHECKOK(group->meth->
			   field_mul(&tempy, &lambda, &tempy, group->meth));
	MP_CHECKOK(group->meth->field_sub(&tempy, qy, &tempy, group->meth));
	MP_CHECKOK(mp_copy(&tempx, rx));
	MP_CHECKOK(mp_copy(&tempy, ry));

  CLEANUP:
	mp_clear(&lambda);
	mp_clear(&temp);
	mp_clear(&tempx);
	mp_clear(&tempy);
	return res;
}

/* Computes R = P - Q. Elliptic curve points P, Q, and R can all be
 * identical. Uses affine coordinates. Assumes input is already
 * field-encoded using field_enc, and returns output that is still
 * field-encoded. */
mp_err
ec_GFp_pt_sub_aff(const mp_int *px, const mp_int *py, const mp_int *qx,
				  const mp_int *qy, mp_int *rx, mp_int *ry,
				  const ECGroup *group)
{
	mp_err res = MP_OKAY;
	mp_int nqy;

	MP_DIGITS(&nqy) = 0;
	MP_CHECKOK(mp_init(&nqy));
	/* nqy = -qy */
	MP_CHECKOK(group->meth->field_neg(qy, &nqy, group->meth));
	res = group->point_add(px, py, qx, &nqy, rx, ry, group);
  CLEANUP:
	mp_clear(&nqy);
	return res;
}

/* Computes R = 2P. Elliptic curve points P and R can be identical. Uses
 * affine coordinates. Assumes input is already field-encoded using
 * field_enc, and returns output that is still field-encoded. */
mp_err
ec_GFp_pt_dbl_aff(const mp_int *px, const mp_int *py, mp_int *rx,
				  mp_int *ry, const ECGroup *group)
{
	return ec_GFp_pt_add_aff(px, py, px, py, rx, ry, group);
}

