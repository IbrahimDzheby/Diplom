#pragma once

#ifdef __cplusplus
#define EXTERN_C_CC extern "C"
#else
#define EXTERN_C_CC
#endif

#if defined _WIN32
#define DLL_EXPORT_CC EXTERN_C_CC __declspec(dllexport)
#elif __GNUC__ >= 4
#define DLL_EXPORT_CC EXTERN_C_CC  __attribute__ ((visibility ("default")))
#else
#define DLL_EXPORT_CC EXTERN_C_CC
#endif


/* Custom Headers */
#include <string.h>
#include "mwmathutil.h"

/* Type Definitions */
#ifndef c2_struct_c2_tag_pBB7ZkhZBuLOBvLytCTMVH
#define c2_struct_c2_tag_pBB7ZkhZBuLOBvLytCTMVH
struct c2_tag_pBB7ZkhZBuLOBvLytCTMVH
{
    boolean_T matlabCodegenIsDeleted;
    void *delTri;
    void *convHull;
    void *convHullTri;
};
#endif /* c2_struct_c2_tag_pBB7ZkhZBuLOBvLytCTMVH */
#ifndef c2_typedef_c2_coder_internal_qhullStructsWrapper
#define c2_typedef_c2_coder_internal_qhullStructsWrapper
typedef struct c2_tag_pBB7ZkhZBuLOBvLytCTMVH c2_coder_internal_qhullStructsWrapper;
#endif /* c2_typedef_c2_coder_internal_qhullStructsWrapper */
#ifndef c2_struct_c2_coder_array_real_T_671
#define c2_struct_c2_coder_array_real_T_671
struct c2_coder_array_real_T_671
{
    real_T data[671];
    int32_T size[1];
};
#endif /* c2_struct_c2_coder_array_real_T_671 */
#ifndef c2_typedef_c2_coder_array_real_T_671
#define c2_typedef_c2_coder_array_real_T_671
typedef struct c2_coder_array_real_T_671 c2_coder_array_real_T_671;
#endif /* c2_typedef_c2_coder_array_real_T_671 */
#ifndef c2_struct_c2_coder_array_real_T_2x671_2D
#define c2_struct_c2_coder_array_real_T_2x671_2D
struct c2_coder_array_real_T_2x671_2D
{
    real_T data[1342];
    int32_T size[2];
};
#endif /* c2_struct_c2_coder_array_real_T_2x671_2D */
#ifndef c2_typedef_c2_coder_array_real_T_2x671
#define c2_typedef_c2_coder_array_real_T_2x671
typedef struct c2_coder_array_real_T_2x671_2D c2_coder_array_real_T_2x671;
#endif /* c2_typedef_c2_coder_array_real_T_2x671 */
#ifndef c2_struct_c2_tag_NXZUxKoBXzDCgOAOVVFG6B
#define c2_struct_c2_tag_NXZUxKoBXzDCgOAOVVFG6B
struct c2_tag_NXZUxKoBXzDCgOAOVVFG6B
{
    int32_T numPts;
    c2_coder_array_real_T_2x671 thePoints;
    c2_coder_internal_qhullStructsWrapper *qhWrapper;
    real_T idxMap[671];
    boolean_T dupesExist;
};
#endif /* c2_struct_c2_tag_NXZUxKoBXzDCgOAOVVFG6B */
#ifndef c2_typedef_c2_coder_internal_delaunayTriangulation
#define c2_typedef_c2_coder_internal_delaunayTriangulation
typedef struct c2_tag_NXZUxKoBXzDCgOAOVVFG6B c2_coder_internal_delaunayTriangulation;
#endif /* c2_typedef_c2_coder_internal_delaunayTriangulation */
#ifndef c2_struct_c2_tag_GHb4BqQAGj2KtXuLxNlCRE
#define c2_struct_c2_tag_GHb4BqQAGj2KtXuLxNlCRE
struct c2_tag_GHb4BqQAGj2KtXuLxNlCRE
{
    c2_coder_array_real_T_671 sampleVal;
    c2_coder_internal_delaunayTriangulation delTri;
    boolean_T validInterpolant;
};
#endif /* c2_struct_c2_tag_GHb4BqQAGj2KtXuLxNlCRE */
#ifndef c2_typedef_c2_scatteredInterpolant
#define c2_typedef_c2_scatteredInterpolant
typedef struct c2_tag_GHb4BqQAGj2KtXuLxNlCRE c2_scatteredInterpolant;
#endif /* c2_typedef_c2_scatteredInterpolant */
#ifndef typedef_gvar_instance
#define typedef_gvar_instance
typedef struct
{
    uint8_T c2_JITStateAnimation[1];
    real_T *c2_rpm;
    real_T *c2_bsfc;
    real_T *c2_torque;
    c2_scatteredInterpolant c2_F;
    boolean_T c2_F_not_empty;
    uint8_T c2_JITTransitionAnimation[1];
    emlrtRSInfo c2_ab_emlrtRSI;
    emlrtMCInfo c2_b_emlrtMCI;
    emlrtRSInfo c2_b_emlrtRSI;
    emlrtRTEInfo c2_b_emlrtRTEI;
    emlrtRSInfo c2_bb_emlrtRSI;
    emlrtMCInfo c2_c_emlrtMCI;
    emlrtRSInfo c2_c_emlrtRSI;
    emlrtRTEInfo c2_c_emlrtRTEI;
    emlrtRSInfo c2_cb_emlrtRSI;
    emlrtMCInfo c2_d_emlrtMCI;
    emlrtRSInfo c2_d_emlrtRSI;
    emlrtRTEInfo c2_d_emlrtRTEI;
    emlrtRSInfo c2_db_emlrtRSI;
    emlrtMCInfo c2_e_emlrtMCI;
    emlrtRSInfo c2_e_emlrtRSI;
    emlrtRTEInfo c2_e_emlrtRTEI;
    emlrtRSInfo c2_eb_emlrtRSI;
    emlrtMCInfo c2_emlrtMCI;
    emlrtRSInfo c2_emlrtRSI;
    emlrtRTEInfo c2_emlrtRTEI;
    void *c2_fEmlrtCtx;
    emlrtRSInfo c2_f_emlrtRSI;
    emlrtRTEInfo c2_f_emlrtRTEI;
    emlrtRSInfo c2_fb_emlrtRSI;
    emlrtRSInfo c2_g_emlrtRSI;
    emlrtRTEInfo c2_g_emlrtRTEI;
    emlrtRSInfo c2_gb_emlrtRSI;
    c2_coder_internal_qhullStructsWrapper c2_gobj_0;
    emlrtRSInfo c2_h_emlrtRSI;
    emlrtRTEInfo c2_h_emlrtRTEI;
    emlrtRSInfo c2_hb_emlrtRSI;
    emlrtRSInfo c2_i_emlrtRSI;
    emlrtRTEInfo c2_i_emlrtRTEI;
    emlrtRSInfo c2_ib_emlrtRSI;
    emlrtRSInfo c2_j_emlrtRSI;
    emlrtRTEInfo c2_j_emlrtRTEI;
    emlrtRSInfo c2_jb_emlrtRSI;
    emlrtRSInfo c2_k_emlrtRSI;
    emlrtRTEInfo c2_k_emlrtRTEI;
    emlrtRSInfo c2_l_emlrtRSI;
    emlrtRTEInfo c2_l_emlrtRTEI;
    emlrtRSInfo c2_m_emlrtRSI;
    emlrtRTEInfo c2_m_emlrtRTEI;
    emlrtRSInfo c2_n_emlrtRSI;
    emlrtRTEInfo c2_n_emlrtRTEI;
    emlrtRSInfo c2_o_emlrtRSI;
    emlrtRTEInfo c2_o_emlrtRTEI;
    emlrtRSInfo c2_p_emlrtRSI;
    emlrtRSInfo c2_q_emlrtRSI;
    emlrtRSInfo c2_r_emlrtRSI;
    real_T c2_rpm_max;
    boolean_T c2_rpm_max_not_empty;
    real_T c2_rpm_min;
    boolean_T c2_rpm_min_not_empty;
    emlrtRSInfo c2_s_emlrtRSI;
    emlrtRSInfo c2_t_emlrtRSI;
    real_T c2_torque_max;
    boolean_T c2_torque_max_not_empty;
    real_T c2_torque_min;
    boolean_T c2_torque_min_not_empty;
    emlrtRSInfo c2_u_emlrtRSI;
    emlrtRSInfo c2_v_emlrtRSI;
    emlrtRSInfo c2_w_emlrtRSI;
    emlrtRSInfo c2_x_emlrtRSI;
    emlrtRSInfo c2_y_emlrtRSI;
} gvar_instance;
#endif /* typedef_gvar_instance */

/* Named Constants */

/* Variable Declarations */

/* Variable Definitions */

/* Function Declarations */
DLL_EXPORT_CC void initialize_c2_BSFC_simulink(SimStruct *S, gvar_instance *ptr_gvar_instance);
DLL_EXPORT_CC void initialize_params_c2_BSFC_simulink(SimStruct *S, gvar_instance *ptr_gvar_instance);
DLL_EXPORT_CC void mdl_start_c2_BSFC_simulink(SimStruct *S, gvar_instance *ptr_gvar_instance);
DLL_EXPORT_CC void mdl_terminate_c2_BSFC_simulink(SimStruct *S, gvar_instance *ptr_gvar_instance);
DLL_EXPORT_CC void mdl_setup_runtime_resources_c2_BSFC_simulink(SimStruct *S, gvar_instance *ptr_gvar_instance);
DLL_EXPORT_CC void mdl_cleanup_runtime_resources_c2_BSFC_simulink(SimStruct *S, gvar_instance *ptr_gvar_instance);
DLL_EXPORT_CC void enable_c2_BSFC_simulink(SimStruct *S, gvar_instance *ptr_gvar_instance);
DLL_EXPORT_CC void disable_c2_BSFC_simulink(SimStruct *S, gvar_instance *ptr_gvar_instance);
DLL_EXPORT_CC void sf_gateway_c2_BSFC_simulink(SimStruct *S, gvar_instance *ptr_gvar_instance);
DLL_EXPORT_CC void ext_mode_exec_c2_BSFC_simulink(SimStruct *S, gvar_instance *ptr_gvar_instance);
DLL_EXPORT_CC const mxArray *get_sim_state_c2_BSFC_simulink(SimStruct *S, gvar_instance *ptr_gvar_instance);
DLL_EXPORT_CC void set_sim_state_c2_BSFC_simulink(SimStruct *S, gvar_instance *ptr_gvar_instance, const mxArray *c2_st);
DLL_EXPORT_CC boolean_T c2_sortLE(SimStruct *S, gvar_instance *ptr_gvar_instance, real_T c2_v[1342], int32_T c2_idx1, int32_T c2_idx2);
DLL_EXPORT_CC void c2_scatteredInterpolant_scatteredInterpolant(SimStruct *S, gvar_instance *ptr_gvar_instance, const emlrtStack *c2_sp, c2_coder_internal_qhullStructsWrapper *c2_iobj_0, real_T c2_obj_sampleVal_data[], int32_T c2_obj_sampleVal_size[1], int32_T *c2_obj_delTri_numPts, real_T c2_obj_delTri_thePoints_data[], int32_T c2_obj_delTri_thePoints_size[2], c2_coder_internal_qhullStructsWrapper **c2_obj_delTri_qhWrapper, real_T c2_obj_delTri_idxMap[671], boolean_T *c2_obj_delTri_dupesExist, boolean_T *c2_obj_validInterpolant);
DLL_EXPORT_CC void c2_warning(SimStruct *S, gvar_instance *ptr_gvar_instance, const emlrtStack *c2_sp);
DLL_EXPORT_CC void c2_b_warning(SimStruct *S, gvar_instance *ptr_gvar_instance, const emlrtStack *c2_sp);
DLL_EXPORT_CC real_T c2_emlrt_marshallIn(SimStruct *S, gvar_instance *ptr_gvar_instance, const mxArray *c2_nullptr, const char_T *c2_identifier);
DLL_EXPORT_CC real_T c2_b_emlrt_marshallIn(SimStruct *S, gvar_instance *ptr_gvar_instance, const mxArray *c2_u, const emlrtMsgIdentifier *c2_parentId);
DLL_EXPORT_CC real_T c2_c_emlrt_marshallIn(SimStruct *S, gvar_instance *ptr_gvar_instance, const mxArray *c2_nullptr, const char_T *c2_identifier, boolean_T *c2_svPtr);
DLL_EXPORT_CC real_T c2_d_emlrt_marshallIn(SimStruct *S, gvar_instance *ptr_gvar_instance, const mxArray *c2_u, const emlrtMsgIdentifier *c2_parentId, boolean_T *c2_svPtr);
DLL_EXPORT_CC const mxArray *c2_feval(SimStruct *S, gvar_instance *ptr_gvar_instance, const emlrtStack *c2_sp, const mxArray *c2_input0, const mxArray *c2_input1);
DLL_EXPORT_CC void c2_b_feval(SimStruct *S, gvar_instance *ptr_gvar_instance, const emlrtStack *c2_sp, const mxArray *c2_input0, const mxArray *c2_input1);
DLL_EXPORT_CC const mxArray *c2_c_feval(SimStruct *S, gvar_instance *ptr_gvar_instance, const emlrtStack *c2_sp, const mxArray *c2_input0, const mxArray *c2_input1);
DLL_EXPORT_CC void c2_d_feval(SimStruct *S, gvar_instance *ptr_gvar_instance, const emlrtStack *c2_sp, const mxArray *c2_input0, const mxArray *c2_input1);
DLL_EXPORT_CC void c2_array_scatteredInterpolant_Cons(SimStruct *S, gvar_instance *ptr_gvar_instance, c2_scatteredInterpolant *c2_pStruct);
DLL_EXPORT_CC void c2_array_real_T_671_Constructor(SimStruct *S, gvar_instance *ptr_gvar_instance, c2_coder_array_real_T_671 *c2_pArray);
DLL_EXPORT_CC void c2_array_coder_internal_delaunayTr(SimStruct *S, gvar_instance *ptr_gvar_instance, c2_coder_internal_delaunayTriangulation *c2_pStruct);
DLL_EXPORT_CC void c2_array_real_T_2x671_Constructor(SimStruct *S, gvar_instance *ptr_gvar_instance, c2_coder_array_real_T_2x671 *c2_pArray);
DLL_EXPORT_CC void init_dsm_address_info(SimStruct *S, gvar_instance *ptr_gvar_instance);
DLL_EXPORT_CC void init_simulink_io_address(SimStruct *S, gvar_instance *ptr_gvar_instance);
DLL_EXPORT_CC void JIT_release_mem_fcn(gvar_instance *ptr_gvar_instance);
DLL_EXPORT_CC gvar_instance *JIT_init_mem_fcn(void);

/* Function Definitions */

