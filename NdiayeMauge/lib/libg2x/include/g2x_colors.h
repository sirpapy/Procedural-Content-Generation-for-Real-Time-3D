/* ===============================================================
  E.Incerti - Université de Marne-la-Vallée - incerti@univ-mlv.fr 
       - Librairie G2X - Fonctions de base d'accès public -       
	                  version 5.0 - Dec.2012                        
  ============================================================= */

#ifdef __cplusplus
	extern "C" {
#endif

#ifndef _G2X_COLORS_H
  #define _G2X_COLORS_H
  
	/* DES COULEURS RGBA PREDEFINIES                           */
	/* les 6 teintes primaires (RGB) et secondaires (CMY)      */
	/* plus Orange, blanc (->gris) et , noir                   */
	/* 5 niveaux d'intensité 100%,75%,50%,33%,16%              */
	/* 4 niveaux de densité (transp.) 100%,75%,50%,25%         */
	
	/* densité   100%                                          */
	/* intensité 100%                                          */
	#define G2Xw    (G2Xcolor){1.00,1.00,1.00,1.00} /* blanc   */
	#define G2Xr    (G2Xcolor){1.00,0.00,0.00,1.00} /* rouge   */
	#define G2Xo    (G2Xcolor){1.00,0.50,0.25,1.00} /* orange  */
	#define G2Xy    (G2Xcolor){1.00,1.00,0.00,1.00} /* jaune   */
	#define G2Xg    (G2Xcolor){0.00,1.00,0.00,1.00} /* vert    */
	#define G2Xc    (G2Xcolor){0.00,1.00,1.00,1.00} /* cyan    */
	#define G2Xb    (G2Xcolor){0.00,0.00,1.00,1.00} /* bleu    */
	#define G2Xm    (G2Xcolor){1.00,0.00,1.00,1.00} /* magenta */
	#define G2Xk    (G2Xcolor){0.00,0.00,0.00,1.00} /* noir    */
	/* intensité  75%                                          */
	#define G2Xwa   (G2Xcolor){0.75,0.75,0.75,1.00} /*         */
	#define G2Xra   (G2Xcolor){0.75,0.00,0.00,1.00} /*         */
	#define G2Xoa   (G2Xcolor){0.75,0.37,0.12,1.00} /*         */
	#define G2Xya   (G2Xcolor){0.75,0.75,0.00,1.00} /*         */
	#define G2Xga   (G2Xcolor){0.00,0.75,0.00,1.00} /*         */
	#define G2Xca   (G2Xcolor){0.00,0.75,0.75,1.00} /*         */
	#define G2Xba   (G2Xcolor){0.00,0.00,0.75,1.00} /*         */
	#define G2Xma   (G2Xcolor){0.75,0.00,0.75,1.00} /*         */
	/* intensité  50%                                          */
	#define G2Xwb   (G2Xcolor){0.50,0.50,0.50,1.00} /*         */
	#define G2Xrb   (G2Xcolor){0.50,0.12,0.12,1.00} /*         */
	#define G2Xob   (G2Xcolor){0.50,0.25,0.06,1.00} /*         */
	#define G2Xyb   (G2Xcolor){0.50,0.50,0.12,1.00} /*         */
	#define G2Xgb   (G2Xcolor){0.12,0.50,0.12,1.00} /*         */
	#define G2Xcb   (G2Xcolor){0.12,0.50,0.50,1.00} /*         */
	#define G2Xbb   (G2Xcolor){0.12,0.12,0.50,1.00} /*         */
	#define G2Xmb   (G2Xcolor){0.50,0.12,0.50,1.00} /*         */
	/* intensité  33%                                          */
	#define G2Xwc   (G2Xcolor){0.33,0.33,0.33,1.00} /*         */
	#define G2Xrc   (G2Xcolor){0.33,0.06,0.06,1.00} /*         */
	#define G2Xoc   (G2Xcolor){0.33,0.16,0.06,1.00} /*         */
	#define G2Xyc   (G2Xcolor){0.33,0.33,0.06,1.00} /*         */
	#define G2Xgc   (G2Xcolor){0.06,0.33,0.06,1.00} /*         */
	#define G2Xcc   (G2Xcolor){0.06,0.33,0.33,1.00} /*         */
	#define G2Xbc   (G2Xcolor){0.06,0.06,0.33,1.00} /*         */
	#define G2Xmc   (G2Xcolor){0.33,0.06,0.33,1.00} /*         */
	/* intensité  16%                                          */
	#define G2Xwd   (G2Xcolor){0.16,0.12,0.12,1.00} /*         */
	#define G2Xrd   (G2Xcolor){0.16,0.04,0.04,1.00} /*         */
	#define G2Xod   (G2Xcolor){0.16,0.08,0.04,1.00} /*         */
	#define G2Xyd   (G2Xcolor){0.16,0.16,0.04,1.00} /*         */
	#define G2Xgd   (G2Xcolor){0.04,0.16,0.04,1.00} /*         */
	#define G2Xcd   (G2Xcolor){0.04,0.16,0.16,1.00} /*         */
	#define G2Xbd   (G2Xcolor){0.04,0.04,0.16,1.00} /*         */
	#define G2Xmd   (G2Xcolor){0.16,0.04,0.16,1.00} /*         */


	/* densité    75%                                          */
	/* intensité 100%                                          */
	#define G2Xw_a  (G2Xcolor){1.00,1.00,1.00,0.75} /* blanc   */
	#define G2Xr_a  (G2Xcolor){1.00,0.00,0.00,0.75} /* rouge   */
	#define G2Xo_a  (G2Xcolor){1.00,0.50,0.25,0.75} /* orange  */
	#define G2Xy_a  (G2Xcolor){1.00,1.00,0.00,0.75} /* jaune   */
	#define G2Xg_a  (G2Xcolor){0.00,1.00,0.00,0.75} /* vert    */
	#define G2Xc_a  (G2Xcolor){0.00,1.00,1.00,0.75} /* cyan    */
	#define G2Xb_a  (G2Xcolor){0.00,0.00,1.00,0.75} /* bleu    */
	#define G2Xm_a  (G2Xcolor){1.00,0.00,1.00,0.75} /* magenta */
	#define G2Xk_a  (G2Xcolor){0.00,0.00,0.00,0.75} /* noir    */
	/* intensité  75%                                          */
	#define G2Xwa_a (G2Xcolor){0.75,0.75,0.75,0.75} /*         */
	#define G2Xra_a (G2Xcolor){0.75,0.00,0.00,0.75} /*         */
	#define G2Xoa_a (G2Xcolor){0.75,0.37,0.12,0.75} /*         */
	#define G2Xya_a (G2Xcolor){0.75,0.75,0.00,0.75} /*         */
	#define G2Xga_a (G2Xcolor){0.00,0.75,0.00,0.75} /*         */
	#define G2Xca_a (G2Xcolor){0.00,0.75,0.75,0.75} /*         */
	#define G2Xba_a (G2Xcolor){0.00,0.00,0.75,0.75} /*         */
	#define G2Xma_a (G2Xcolor){0.75,0.00,0.75,0.75} /*         */
	/* intensité  50%                                          */
	#define G2Xwb_a (G2Xcolor){0.50,0.50,0.50,0.75} /*         */
	#define G2Xrb_a (G2Xcolor){0.50,0.12,0.12,0.75} /*         */
	#define G2Xob_a (G2Xcolor){0.50,0.25,0.06,0.75} /*         */
	#define G2Xyb_a (G2Xcolor){0.50,0.50,0.12,0.75} /*         */
	#define G2Xgb_a (G2Xcolor){0.12,0.50,0.12,0.75} /*         */
	#define G2Xcb_a (G2Xcolor){0.12,0.50,0.50,0.75} /*         */
	#define G2Xbb_a (G2Xcolor){0.12,0.12,0.50,0.75} /*         */
	#define G2Xmb_a (G2Xcolor){0.50,0.12,0.50,0.75} /*         */
	/* intensité  33%                                          */
	#define G2Xwc_a (G2Xcolor){0.33,0.33,0.33,0.75} /*         */
	#define G2Xrc_a (G2Xcolor){0.33,0.06,0.06,0.75} /*         */
	#define G2Xoc_a (G2Xcolor){0.33,0.16,0.06,0.75} /*         */
	#define G2Xyc_a (G2Xcolor){0.33,0.33,0.06,0.75} /*         */
	#define G2Xgc_a (G2Xcolor){0.06,0.33,0.06,0.75} /*         */
	#define G2Xcc_a (G2Xcolor){0.06,0.33,0.33,0.75} /*         */
	#define G2Xbc_a (G2Xcolor){0.06,0.06,0.33,0.75} /*         */
	#define G2Xmc_a (G2Xcolor){0.33,0.06,0.33,0.75} /*         */
	/* intensité  16%                                          */
	#define G2Xwd_a (G2Xcolor){0.16,0.12,0.12,0.75} /*         */
	#define G2Xrd_a (G2Xcolor){0.16,0.04,0.04,0.75} /*         */
	#define G2Xod_a (G2Xcolor){0.16,0.08,0.04,0.75} /*         */
	#define G2Xyd_a (G2Xcolor){0.16,0.16,0.04,0.75} /*         */
	#define G2Xgd_a (G2Xcolor){0.04,0.16,0.04,0.75} /*         */
	#define G2Xcd_a (G2Xcolor){0.04,0.16,0.16,0.75} /*         */
	#define G2Xbd_a (G2Xcolor){0.04,0.04,0.16,0.75} /*         */
	#define G2Xmd_a (G2Xcolor){0.16,0.04,0.16,0.75} /*         */

	/* densité    50%                                          */
	/* intensité 100%                                          */
	#define G2Xw_b  (G2Xcolor){1.00,1.00,1.00,0.50} /* blanc   */
	#define G2Xr_b  (G2Xcolor){1.00,0.00,0.00,0.50} /* rouge   */
	#define G2Xo_b  (G2Xcolor){1.00,0.50,0.25,0.50} /* orange  */
	#define G2Xy_b  (G2Xcolor){1.00,1.00,0.00,0.50} /* jaune   */
	#define G2Xg_b  (G2Xcolor){0.00,1.00,0.00,0.50} /* vert    */
	#define G2Xc_b  (G2Xcolor){0.00,1.00,1.00,0.50} /* cyan    */
	#define G2Xb_b  (G2Xcolor){0.00,0.00,1.00,0.50} /* bleu    */
	#define G2Xm_b  (G2Xcolor){1.00,0.00,1.00,0.50} /* magenta */
	#define G2Xk_b  (G2Xcolor){0.00,0.00,0.00,0.50} /* noir    */
	/* intensité  75%                                          */
	#define G2Xwa_b (G2Xcolor){0.75,0.75,0.75,0.50} /*         */
	#define G2Xra_b (G2Xcolor){0.75,0.00,0.00,0.50} /*         */
	#define G2Xoa_b (G2Xcolor){0.75,0.37,0.12,0.50} /*         */
	#define G2Xya_b (G2Xcolor){0.75,0.75,0.00,0.50} /*         */
	#define G2Xga_b (G2Xcolor){0.00,0.75,0.00,0.50} /*         */
	#define G2Xca_b (G2Xcolor){0.00,0.75,0.75,0.50} /*         */
	#define G2Xba_b (G2Xcolor){0.00,0.00,0.75,0.50} /*         */
	#define G2Xma_b (G2Xcolor){0.75,0.00,0.75,0.50} /*         */
	/* intensité  50%                                          */
	#define G2Xwb_b (G2Xcolor){0.50,0.50,0.50,0.50} /*         */
	#define G2Xrb_b (G2Xcolor){0.50,0.12,0.12,0.50} /*         */
	#define G2Xob_b (G2Xcolor){0.50,0.25,0.06,0.50} /*         */
	#define G2Xyb_b (G2Xcolor){0.50,0.50,0.12,0.50} /*         */
	#define G2Xgb_b (G2Xcolor){0.12,0.50,0.12,0.50} /*         */
	#define G2Xcb_b (G2Xcolor){0.12,0.50,0.50,0.50} /*         */
	#define G2Xbb_b (G2Xcolor){0.12,0.12,0.50,0.50} /*         */
	#define G2Xmb_b (G2Xcolor){0.50,0.12,0.50,0.50} /*         */
	/* intensité  33%                                          */
	#define G2Xwc_b (G2Xcolor){0.33,0.33,0.33,0.50} /*         */
	#define G2Xrc_b (G2Xcolor){0.33,0.06,0.06,0.50} /*         */
	#define G2Xoc_b (G2Xcolor){0.33,0.16,0.06,0.50} /*         */
	#define G2Xyc_b (G2Xcolor){0.33,0.33,0.06,0.50} /*         */
	#define G2Xgc_b (G2Xcolor){0.06,0.33,0.06,0.50} /*         */
	#define G2Xcc_b (G2Xcolor){0.06,0.33,0.33,0.50} /*         */
	#define G2Xbc_b (G2Xcolor){0.06,0.06,0.33,0.50} /*         */
	#define G2Xmc_b (G2Xcolor){0.33,0.06,0.33,0.50} /*         */
	/* intensité  16%                                          */
	#define G2Xwd_b (G2Xcolor){0.16,0.12,0.12,0.50} /*         */
	#define G2Xrd_b (G2Xcolor){0.16,0.04,0.04,0.50} /*         */
	#define G2Xod_b (G2Xcolor){0.16,0.08,0.04,0.50} /*         */
	#define G2Xyd_b (G2Xcolor){0.16,0.16,0.04,0.50} /*         */
	#define G2Xgd_b (G2Xcolor){0.04,0.16,0.04,0.50} /*         */
	#define G2Xcd_b (G2Xcolor){0.04,0.16,0.16,0.50} /*         */
	#define G2Xbd_b (G2Xcolor){0.04,0.04,0.16,0.50} /*         */
	#define G2Xmd_b (G2Xcolor){0.16,0.04,0.16,0.50} /*         */

	/* densité    25%                                          */
	/* intensité 100%                                          */
	#define G2Xw_c  (G2Xcolor){1.00,1.00,1.00,0.25} /* blanc   */
	#define G2Xr_c  (G2Xcolor){1.00,0.00,0.00,0.25} /* rouge   */
	#define G2Xo_c  (G2Xcolor){1.00,0.50,0.25,0.25} /* orange  */
	#define G2Xy_c  (G2Xcolor){1.00,1.00,0.00,0.25} /* jaune   */
	#define G2Xg_c  (G2Xcolor){0.00,1.00,0.00,0.25} /* vert    */
	#define G2Xc_c  (G2Xcolor){0.00,1.00,1.00,0.25} /* cyan    */
	#define G2Xb_c  (G2Xcolor){0.00,0.00,1.00,0.25} /* bleu    */
	#define G2Xm_c  (G2Xcolor){1.00,0.00,1.00,0.25} /* magenta */
	#define G2Xk_c  (G2Xcolor){0.00,0.00,0.00,0.25} /* noir    */
	/* intensité  75%                                          */
	#define G2Xwa_c (G2Xcolor){0.75,0.75,0.75,0.25} /*         */
	#define G2Xra_c (G2Xcolor){0.75,0.00,0.00,0.25} /*         */
	#define G2Xoa_c (G2Xcolor){0.75,0.37,0.12,0.25} /*         */
	#define G2Xya_c (G2Xcolor){0.75,0.75,0.00,0.25} /*         */
	#define G2Xga_c (G2Xcolor){0.00,0.75,0.00,0.25} /*         */
	#define G2Xca_c (G2Xcolor){0.00,0.75,0.75,0.25} /*         */
	#define G2Xba_c (G2Xcolor){0.00,0.00,0.75,0.25} /*         */
	#define G2Xma_c (G2Xcolor){0.75,0.00,0.75,0.25} /*         */
	/* intensité  50%                                          */
	#define G2Xwb_c (G2Xcolor){0.50,0.50,0.50,0.25} /*         */
	#define G2Xrb_c (G2Xcolor){0.50,0.12,0.12,0.25} /*         */
	#define G2Xob_c (G2Xcolor){0.50,0.25,0.06,0.25} /*         */
	#define G2Xyb_c (G2Xcolor){0.50,0.50,0.12,0.25} /*         */
	#define G2Xgb_c (G2Xcolor){0.12,0.50,0.12,0.25} /*         */
	#define G2Xcb_c (G2Xcolor){0.12,0.50,0.50,0.25} /*         */
	#define G2Xbb_c (G2Xcolor){0.12,0.12,0.50,0.25} /*         */
	#define G2Xmb_c (G2Xcolor){0.50,0.12,0.50,0.25} /*         */
	/* intensité  33%                                          */
	#define G2Xwc_c (G2Xcolor){0.33,0.33,0.33,0.25} /*         */
	#define G2Xrc_c (G2Xcolor){0.33,0.06,0.06,0.25} /*         */
	#define G2Xoc_c (G2Xcolor){0.33,0.16,0.06,0.25} /*         */
	#define G2Xyc_c (G2Xcolor){0.33,0.33,0.06,0.25} /*         */
	#define G2Xgc_c (G2Xcolor){0.06,0.33,0.06,0.25} /*         */
	#define G2Xcc_c (G2Xcolor){0.06,0.33,0.33,0.25} /*         */
	#define G2Xbc_c (G2Xcolor){0.06,0.06,0.33,0.25} /*         */
	#define G2Xmc_c (G2Xcolor){0.33,0.06,0.33,0.25} /*         */
	/* intensité  16%                                         */
	#define G2Xwd_c (G2Xcolor){0.16,0.12,0.12,0.25} /*         */
	#define G2Xrd_c (G2Xcolor){0.16,0.04,0.04,0.25} /*         */
	#define G2Xod_c (G2Xcolor){0.16,0.08,0.04,0.25} /*         */
	#define G2Xyd_c (G2Xcolor){0.16,0.16,0.04,0.25} /*         */
	#define G2Xgd_c (G2Xcolor){0.04,0.16,0.04,0.25} /*         */
	#define G2Xcd_c (G2Xcolor){0.04,0.16,0.16,0.25} /*         */
	#define G2Xbd_c (G2Xcolor){0.04,0.04,0.16,0.25} /*         */
	#define G2Xmd_c (G2Xcolor){0.16,0.04,0.16,0.25} /*         */

#endif

#ifdef __cplusplus
	}
#endif
/*=============================================================================*/
