#define S_FUNCTION_NAME sf_sfun
#include "covrt.h"
#include "cgxert.h"
#include "emlrt.h"
#include "sfrtif/sfc_sf.h"
#include "sfrtif/MessageServiceLayer.h"
#include "sfrtif/DebuggerRuntimeInterface.h"
#include "sfrtif/sfc_mex.h"
#include "sfrtif/sf_runtime_errors.h"
#include "sfrtif/sf_partitioning_execution_bridge.h"
#include "rtwtypes.h"
#include "simtarget/slSimTgtClientServerAPIBridge.h"
#include "sfrtif/sfc_sdi.h"
#include "sfrtif/sf_test_language.h"
#include "simlogCIntrf.h"
#include "half_type.h"
#include "multiword_types.h"
#include "sfrtif/sfc_messages.h"
#include "slccrt.h"
#include "sl_sfcn_cov/sl_sfcn_cov_bridge.h"
#include "mwstringutil.h"
#include "blas.h"
#include "lapacke.h"
#include "s2hwjUlHhgzf62R80BHDxSB.h"
#include "scatteredInterp_qhull_util.h"

#define rtInf (mxGetInf())
#define rtMinusInf (-(mxGetInf()))
#define rtNaN (mxGetNaN())
#define rtInfF ((real32_T)mxGetInf())
#define rtMinusInfF (-(real32_T)mxGetInf())
#define rtNaNF ((real32_T)mxGetNaN())
#define rtIsNaN(X) ((int)mxIsNaN(X))
#define rtIsInf(X) ((int)mxIsInf(X))
#ifdef utFree
#undef utFree
#endif
#ifdef utMalloc
#undef utMalloc
#endif
#ifdef __cplusplus
extern "C" void* utMalloc(size_t size);
extern "C" void utFree(void*);
#else
extern void* utMalloc(size_t size);
extern void utFree(void*);
#endif


/* Type Definitions */
#ifndef c2_struct_c2_tag_spGKsvEVm7uA89hv31XX4LH
#define c2_struct_c2_tag_spGKsvEVm7uA89hv31XX4LH
struct c2_tag_spGKsvEVm7uA89hv31XX4LH
{
    uint32_T MissingPlacement;
    uint32_T ComparisonMethod;
};
#endif /* c2_struct_c2_tag_spGKsvEVm7uA89hv31XX4LH */
#ifndef c2_typedef_c2_spGKsvEVm7uA89hv31XX4LH
#define c2_typedef_c2_spGKsvEVm7uA89hv31XX4LH
typedef struct c2_tag_spGKsvEVm7uA89hv31XX4LH c2_spGKsvEVm7uA89hv31XX4LH;
#endif /* c2_typedef_c2_spGKsvEVm7uA89hv31XX4LH */
#ifndef c2_struct_c2_tag_skA4KFEZ4HPkJJBOYCrevdH
#define c2_struct_c2_tag_skA4KFEZ4HPkJJBOYCrevdH
struct c2_tag_skA4KFEZ4HPkJJBOYCrevdH
{
    uint32_T SafeEq;
    uint32_T Absolute;
    uint32_T NaNBias;
    uint32_T NaNWithFinite;
    uint32_T FiniteWithNaN;
    uint32_T NaNWithNaN;
};
#endif /* c2_struct_c2_tag_skA4KFEZ4HPkJJBOYCrevdH */
#ifndef c2_typedef_c2_skA4KFEZ4HPkJJBOYCrevdH
#define c2_typedef_c2_skA4KFEZ4HPkJJBOYCrevdH
typedef struct c2_tag_skA4KFEZ4HPkJJBOYCrevdH c2_skA4KFEZ4HPkJJBOYCrevdH;
#endif /* c2_typedef_c2_skA4KFEZ4HPkJJBOYCrevdH */
#ifndef c2_struct_c2_tag_sc6f4Behc0Ffg9eeZ0XliHC
#define c2_struct_c2_tag_sc6f4Behc0Ffg9eeZ0XliHC
struct c2_tag_sc6f4Behc0Ffg9eeZ0XliHC
{
    uint32_T dim;
    uint32_T nanflag;
    uint32_T linear;
    uint32_T ComparisonMethod;
};
#endif /* c2_struct_c2_tag_sc6f4Behc0Ffg9eeZ0XliHC */
#ifndef c2_typedef_c2_sc6f4Behc0Ffg9eeZ0XliHC
#define c2_typedef_c2_sc6f4Behc0Ffg9eeZ0XliHC
typedef struct c2_tag_sc6f4Behc0Ffg9eeZ0XliHC c2_sc6f4Behc0Ffg9eeZ0XliHC;
#endif /* c2_typedef_c2_sc6f4Behc0Ffg9eeZ0XliHC */
#ifndef c2_struct_c2_tag_sGaAmWJmK5HvPUvd2k3PkCG
#define c2_struct_c2_tag_sGaAmWJmK5HvPUvd2k3PkCG
struct c2_tag_sGaAmWJmK5HvPUvd2k3PkCG
{
    uint32_T nanflag;
    uint32_T ComparisonMethod;
};
#endif /* c2_struct_c2_tag_sGaAmWJmK5HvPUvd2k3PkCG */
#ifndef c2_typedef_c2_sGaAmWJmK5HvPUvd2k3PkCG
#define c2_typedef_c2_sGaAmWJmK5HvPUvd2k3PkCG
typedef struct c2_tag_sGaAmWJmK5HvPUvd2k3PkCG c2_sGaAmWJmK5HvPUvd2k3PkCG;
#endif /* c2_typedef_c2_sGaAmWJmK5HvPUvd2k3PkCG */
#ifndef c2_struct_c2_tag_srT9PtPSzHSklUSsfAABrcG
#define c2_struct_c2_tag_srT9PtPSzHSklUSsfAABrcG
struct c2_tag_srT9PtPSzHSklUSsfAABrcG
{
    real_T all_triangles[2010];
    real_T all_vertices[2013];
};
#endif /* c2_struct_c2_tag_srT9PtPSzHSklUSsfAABrcG */
#ifndef c2_typedef_c2_srT9PtPSzHSklUSsfAABrcG
#define c2_typedef_c2_srT9PtPSzHSklUSsfAABrcG
typedef struct c2_tag_srT9PtPSzHSklUSsfAABrcG c2_srT9PtPSzHSklUSsfAABrcG;
#endif /* c2_typedef_c2_srT9PtPSzHSklUSsfAABrcG */
#ifndef c2_struct_c2_tag_sKGJXMD9VfgqCU44Gm9fvGC
#define c2_struct_c2_tag_sKGJXMD9VfgqCU44Gm9fvGC
struct c2_tag_sKGJXMD9VfgqCU44Gm9fvGC
{
    boolean_T CaseSensitivity;
    boolean_T StructExpand;
    char_T PartialMatching[6];
    boolean_T IgnoreNulls;
};
#endif /* c2_struct_c2_tag_sKGJXMD9VfgqCU44Gm9fvGC */
#ifndef c2_typedef_c2_sKGJXMD9VfgqCU44Gm9fvGC
#define c2_typedef_c2_sKGJXMD9VfgqCU44Gm9fvGC
typedef struct c2_tag_sKGJXMD9VfgqCU44Gm9fvGC c2_sKGJXMD9VfgqCU44Gm9fvGC;
#endif /* c2_typedef_c2_sKGJXMD9VfgqCU44Gm9fvGC */
#ifndef c2_struct_c2_tag_smzGQHcQ1fZcSCW5rtLpn4F
#define c2_struct_c2_tag_smzGQHcQ1fZcSCW5rtLpn4F
struct c2_tag_smzGQHcQ1fZcSCW5rtLpn4F
{
    boolean_T CaseSensitivity;
    char_T PartialMatching[6];
    boolean_T StructExpand;
    boolean_T IgnoreNulls;
    boolean_T SupportOverrides;
};
#endif /* c2_struct_c2_tag_smzGQHcQ1fZcSCW5rtLpn4F */
#ifndef c2_typedef_c2_smzGQHcQ1fZcSCW5rtLpn4F
#define c2_typedef_c2_smzGQHcQ1fZcSCW5rtLpn4F
typedef struct c2_tag_smzGQHcQ1fZcSCW5rtLpn4F c2_smzGQHcQ1fZcSCW5rtLpn4F;
#endif /* c2_typedef_c2_smzGQHcQ1fZcSCW5rtLpn4F */
#ifndef c2_struct_c2_tag_bXQHQ5tMc49lFbz3k7QrBC
#define c2_struct_c2_tag_bXQHQ5tMc49lFbz3k7QrBC
struct c2_tag_bXQHQ5tMc49lFbz3k7QrBC
{
    char_T f1[16];
    char_T f2[16];
};
#endif /* c2_struct_c2_tag_bXQHQ5tMc49lFbz3k7QrBC */
#ifndef c2_typedef_c2_s_bXQHQ5tMc49lFbz3k7QrBC
#define c2_typedef_c2_s_bXQHQ5tMc49lFbz3k7QrBC
typedef struct c2_tag_bXQHQ5tMc49lFbz3k7QrBC c2_s_bXQHQ5tMc49lFbz3k7QrBC;
#endif /* c2_typedef_c2_s_bXQHQ5tMc49lFbz3k7QrBC */
#ifndef c2_struct_c2_tag_FWAx9YmgCYqLkaFiyAr9e
#define c2_struct_c2_tag_FWAx9YmgCYqLkaFiyAr9e
struct c2_tag_FWAx9YmgCYqLkaFiyAr9e
{
    char_T f1[15];
    char_T f2[12];
    char_T f3[15];
    char_T f4[11];
};
#endif /* c2_struct_c2_tag_FWAx9YmgCYqLkaFiyAr9e */
#ifndef c2_typedef_c2_cell_2
#define c2_typedef_c2_cell_2
typedef struct c2_tag_FWAx9YmgCYqLkaFiyAr9e c2_cell_2;
#endif /* c2_typedef_c2_cell_2 */
#ifndef c2_struct_c2_tag_vCroJ4nSCwjPwQBS7RhjeC
#define c2_struct_c2_tag_vCroJ4nSCwjPwQBS7RhjeC
struct c2_tag_vCroJ4nSCwjPwQBS7RhjeC
{
    char_T f1[6];
    char_T f2[8];
    char_T f3[7];
    char_T f4[13];
    char_T f5[13];
    char_T f6[10];
};
#endif /* c2_struct_c2_tag_vCroJ4nSCwjPwQBS7RhjeC */
#ifndef c2_typedef_c2_cell_3
#define c2_typedef_c2_cell_3
typedef struct c2_tag_vCroJ4nSCwjPwQBS7RhjeC c2_cell_3;
#endif /* c2_typedef_c2_cell_3 */
#ifndef c2_struct_c2_tag_xOZlLoGvSrTJr14RWwCqHG
#define c2_struct_c2_tag_xOZlLoGvSrTJr14RWwCqHG
struct c2_tag_xOZlLoGvSrTJr14RWwCqHG
{
    char_T f1[3];
    char_T f2[7];
    char_T f3[6];
};
#endif /* c2_struct_c2_tag_xOZlLoGvSrTJr14RWwCqHG */
#ifndef c2_typedef_c2_cell_4
#define c2_typedef_c2_cell_4
typedef struct c2_tag_xOZlLoGvSrTJr14RWwCqHG c2_cell_4;
#endif /* c2_typedef_c2_cell_4 */
#ifndef c2_struct_c2_tag_6jR4RtbHdjyG00WYqgD5nF
#define c2_struct_c2_tag_6jR4RtbHdjyG00WYqgD5nF
struct c2_tag_6jR4RtbHdjyG00WYqgD5nF
{
    char_T f1[16];
};
#endif /* c2_struct_c2_tag_6jR4RtbHdjyG00WYqgD5nF */
#ifndef c2_typedef_c2_cell_wrap_1
#define c2_typedef_c2_cell_wrap_1
typedef struct c2_tag_6jR4RtbHdjyG00WYqgD5nF c2_cell_wrap_1;
#endif /* c2_typedef_c2_cell_wrap_1 */
#ifndef c2_struct_c2_tag_njgfiHhWBCqqqpWsKZxr7F
#define c2_struct_c2_tag_njgfiHhWBCqqqpWsKZxr7F
struct c2_tag_njgfiHhWBCqqqpWsKZxr7F
{
    char_T f1[15];
    char_T f2[15];
    char_T f3[12];
    char_T f4[11];
    char_T f5[16];
};
#endif /* c2_struct_c2_tag_njgfiHhWBCqqqpWsKZxr7F */
#ifndef c2_typedef_c2_cell_5
#define c2_typedef_c2_cell_5
typedef struct c2_tag_njgfiHhWBCqqqpWsKZxr7F c2_cell_5;
#endif /* c2_typedef_c2_cell_5 */
#ifndef c2_struct_c2_tag_L5JvjW1A13FyCQi5N783sB
#define c2_struct_c2_tag_L5JvjW1A13FyCQi5N783sB
struct c2_tag_L5JvjW1A13FyCQi5N783sB
{
    char_T f1[7];
};
#endif /* c2_struct_c2_tag_L5JvjW1A13FyCQi5N783sB */
#ifndef c2_typedef_c2_cell_wrap_6
#define c2_typedef_c2_cell_wrap_6
typedef struct c2_tag_L5JvjW1A13FyCQi5N783sB c2_cell_wrap_6;
#endif /* c2_typedef_c2_cell_wrap_6 */
#ifndef c2_struct_c2_tag_x1FQ0fE4IJ1IDz5pAzueIE
#define c2_struct_c2_tag_x1FQ0fE4IJ1IDz5pAzueIE
struct c2_tag_x1FQ0fE4IJ1IDz5pAzueIE
{
    char_T f1[3];
};
#endif /* c2_struct_c2_tag_x1FQ0fE4IJ1IDz5pAzueIE */
#ifndef c2_typedef_c2_cell_wrap_7
#define c2_typedef_c2_cell_wrap_7
typedef struct c2_tag_x1FQ0fE4IJ1IDz5pAzueIE c2_cell_wrap_7;
#endif /* c2_typedef_c2_cell_wrap_7 */
#ifndef c2_struct_c2_tag_pPOZHCqhUkni0P8yc94f9F
#define c2_struct_c2_tag_pPOZHCqhUkni0P8yc94f9F
struct c2_tag_pPOZHCqhUkni0P8yc94f9F
{
    c2_s_bXQHQ5tMc49lFbz3k7QrBC _data;
};
#endif /* c2_struct_c2_tag_pPOZHCqhUkni0P8yc94f9F */
#ifndef c2_typedef_c2_s_pPOZHCqhUkni0P8yc94f9F
#define c2_typedef_c2_s_pPOZHCqhUkni0P8yc94f9F
typedef struct c2_tag_pPOZHCqhUkni0P8yc94f9F c2_s_pPOZHCqhUkni0P8yc94f9F;
#endif /* c2_typedef_c2_s_pPOZHCqhUkni0P8yc94f9F */
#ifndef c2_struct_c2_tag_4dijvwFFQt8JouTajf5bDH
#define c2_struct_c2_tag_4dijvwFFQt8JouTajf5bDH
struct c2_tag_4dijvwFFQt8JouTajf5bDH
{
    c2_cell_2 _data;
};
#endif /* c2_struct_c2_tag_4dijvwFFQt8JouTajf5bDH */
#ifndef c2_typedef_c2_s_4dijvwFFQt8JouTajf5bDH
#define c2_typedef_c2_s_4dijvwFFQt8JouTajf5bDH
typedef struct c2_tag_4dijvwFFQt8JouTajf5bDH c2_s_4dijvwFFQt8JouTajf5bDH;
#endif /* c2_typedef_c2_s_4dijvwFFQt8JouTajf5bDH */
#ifndef c2_struct_c2_tag_qRxSe9N4qwJIjT7M2BxNL
#define c2_struct_c2_tag_qRxSe9N4qwJIjT7M2BxNL
struct c2_tag_qRxSe9N4qwJIjT7M2BxNL
{
    c2_cell_3 _data;
};
#endif /* c2_struct_c2_tag_qRxSe9N4qwJIjT7M2BxNL */
#ifndef c2_typedef_c2_s_qRxSe9N4qwJIjT7M2BxNL
#define c2_typedef_c2_s_qRxSe9N4qwJIjT7M2BxNL
typedef struct c2_tag_qRxSe9N4qwJIjT7M2BxNL c2_s_qRxSe9N4qwJIjT7M2BxNL;
#endif /* c2_typedef_c2_s_qRxSe9N4qwJIjT7M2BxNL */
#ifndef c2_struct_c2_tag_JkNjgv3CFjBZhduPupEzEE
#define c2_struct_c2_tag_JkNjgv3CFjBZhduPupEzEE
struct c2_tag_JkNjgv3CFjBZhduPupEzEE
{
    c2_cell_4 _data;
};
#endif /* c2_struct_c2_tag_JkNjgv3CFjBZhduPupEzEE */
#ifndef c2_typedef_c2_s_JkNjgv3CFjBZhduPupEzEE
#define c2_typedef_c2_s_JkNjgv3CFjBZhduPupEzEE
typedef struct c2_tag_JkNjgv3CFjBZhduPupEzEE c2_s_JkNjgv3CFjBZhduPupEzEE;
#endif /* c2_typedef_c2_s_JkNjgv3CFjBZhduPupEzEE */
#ifndef c2_struct_c2_tag_1nlLkVeIuST25DF6il3ApD
#define c2_struct_c2_tag_1nlLkVeIuST25DF6il3ApD
struct c2_tag_1nlLkVeIuST25DF6il3ApD
{
    c2_cell_wrap_1 _data;
};
#endif /* c2_struct_c2_tag_1nlLkVeIuST25DF6il3ApD */
#ifndef c2_typedef_c2_s_1nlLkVeIuST25DF6il3ApD
#define c2_typedef_c2_s_1nlLkVeIuST25DF6il3ApD
typedef struct c2_tag_1nlLkVeIuST25DF6il3ApD c2_s_1nlLkVeIuST25DF6il3ApD;
#endif /* c2_typedef_c2_s_1nlLkVeIuST25DF6il3ApD */
#ifndef c2_struct_c2_tag_uzuPWHtc1cM7ZRTfbsKeiF
#define c2_struct_c2_tag_uzuPWHtc1cM7ZRTfbsKeiF
struct c2_tag_uzuPWHtc1cM7ZRTfbsKeiF
{
    c2_cell_5 _data;
};
#endif /* c2_struct_c2_tag_uzuPWHtc1cM7ZRTfbsKeiF */
#ifndef c2_typedef_c2_s_uzuPWHtc1cM7ZRTfbsKeiF
#define c2_typedef_c2_s_uzuPWHtc1cM7ZRTfbsKeiF
typedef struct c2_tag_uzuPWHtc1cM7ZRTfbsKeiF c2_s_uzuPWHtc1cM7ZRTfbsKeiF;
#endif /* c2_typedef_c2_s_uzuPWHtc1cM7ZRTfbsKeiF */
#ifndef c2_struct_c2_tag_HOps0FrfA6RiWumqewPwZD
#define c2_struct_c2_tag_HOps0FrfA6RiWumqewPwZD
struct c2_tag_HOps0FrfA6RiWumqewPwZD
{
    c2_cell_wrap_6 _data;
};
#endif /* c2_struct_c2_tag_HOps0FrfA6RiWumqewPwZD */
#ifndef c2_typedef_c2_s_HOps0FrfA6RiWumqewPwZD
#define c2_typedef_c2_s_HOps0FrfA6RiWumqewPwZD
typedef struct c2_tag_HOps0FrfA6RiWumqewPwZD c2_s_HOps0FrfA6RiWumqewPwZD;
#endif /* c2_typedef_c2_s_HOps0FrfA6RiWumqewPwZD */
#ifndef c2_struct_c2_tag_sjYWA9KelG1lgIW9XihuxC
#define c2_struct_c2_tag_sjYWA9KelG1lgIW9XihuxC
struct c2_tag_sjYWA9KelG1lgIW9XihuxC
{
    c2_cell_wrap_7 _data;
};
#endif /* c2_struct_c2_tag_sjYWA9KelG1lgIW9XihuxC */
#ifndef c2_typedef_c2_s_sjYWA9KelG1lgIW9XihuxC
#define c2_typedef_c2_s_sjYWA9KelG1lgIW9XihuxC
typedef struct c2_tag_sjYWA9KelG1lgIW9XihuxC c2_s_sjYWA9KelG1lgIW9XihuxC;
#endif /* c2_typedef_c2_s_sjYWA9KelG1lgIW9XihuxC */
#ifndef c2_typedef_c2_coder_internal_interpolate_interpMethodsEnum
#define c2_typedef_c2_coder_internal_interpolate_interpMethodsEnum
typedef uint8_T c2_coder_internal_interpolate_interpMethodsEnum;
#endif /* c2_typedef_c2_coder_internal_interpolate_interpMethodsEnum */

#ifndef c2_c2_coder_internal_interpolate_interpMethodsEnum_constants
#define c2_c2_coder_internal_interpolate_interpMethodsEnum_constants

/* enum c2_coder_internal_interpolate_interpMethodsEnum */
#define c2_coder_internal_interpolate_interpMethodsEnum_LINEAR ((c2_coder_internal_interpolate_interpMethodsEnum)0U)
#define c2_coder_internal_interpolate_interpMethodsEnum_NEAREST ((c2_coder_internal_interpolate_interpMethodsEnum)1U)
#define c2_coder_internal_interpolate_interpMethodsEnum_NONE ((c2_coder_internal_interpolate_interpMethodsEnum)8U)
#define c2_coder_internal_interpolate_interpMethodsEnum_PREVIOUS ((c2_coder_internal_interpolate_interpMethodsEnum)2U)
#define c2_coder_internal_interpolate_interpMethodsEnum_NEXT ((c2_coder_internal_interpolate_interpMethodsEnum)3U)
#define c2_coder_internal_interpolate_interpMethodsEnum_CUBIC ((c2_coder_internal_interpolate_interpMethodsEnum)4U)
#define c2_coder_internal_interpolate_interpMethodsEnum_SPLINE ((c2_coder_internal_interpolate_interpMethodsEnum)5U)
#define c2_coder_internal_interpolate_interpMethodsEnum_PCHIP ((c2_coder_internal_interpolate_interpMethodsEnum)6U)
#define c2_coder_internal_interpolate_interpMethodsEnum_MAKIMA ((c2_coder_internal_interpolate_interpMethodsEnum)7U)
#define c2_coder_internal_interpolate_interpMethodsEnum_NATURAL ((c2_coder_internal_interpolate_interpMethodsEnum)9U)
#define c2_coder_internal_interpolate_interpMethodsEnum_BOUNDARY ((c2_coder_internal_interpolate_interpMethodsEnum)10U)

#endif /* c2_c2_coder_internal_interpolate_interpMethodsEnum_constants */


/* Named Constants */
#define CALL_EVENT (-1)

/* Variable Declarations */

/* Variable Definitions */

/* Function Declarations */

/* Function Definitions */
void initialize_c2_BSFC_simulink(SimStruct *S, gvar_instance *ptr_gvar_instance)
{
    sim_mode_is_external(S);
    ptr_gvar_instance->c2_F_not_empty = false;
    ptr_gvar_instance->c2_rpm_min_not_empty = false;
    ptr_gvar_instance->c2_rpm_max_not_empty = false;
    ptr_gvar_instance->c2_torque_min_not_empty = false;
    ptr_gvar_instance->c2_torque_max_not_empty = false;
    ptr_gvar_instance->c2_gobj_0.matlabCodegenIsDeleted = true;
    sf_get_time(S);
}

void initialize_params_c2_BSFC_simulink(SimStruct *S, gvar_instance *ptr_gvar_instance)
{
    (void)S;
    (void)ptr_gvar_instance;
}

void mdl_start_c2_BSFC_simulink(SimStruct *S, gvar_instance *ptr_gvar_instance)
{
    c2_array_scatteredInterpolant_Cons(S, ptr_gvar_instance, &ptr_gvar_instance->c2_F);
    sim_mode_is_external(S);
}

void mdl_terminate_c2_BSFC_simulink(SimStruct *S, gvar_instance *ptr_gvar_instance)
{
    static char_T c2_c[52] = { 'I', 'n', 't', 'e', 'r', 'n', 'a', 'l', ' ', 'E', 'r', 'r', 'o', 'r', ' ', ':', ' ', 'M', 'e', 'm', 'o', 'r', 'y', ' ', 'L', 'e', 'a', 'k', '!', '!', '!', ' ', 'M', 'a', 'y', ' ', 't', 'h', 'i', 's', ' ', 'n', 'e', 'v', 'e', 'r', ' ', 'o', 'c', 'c', 'u', 'r' };
    static char_T c2_b[23] = { 'C', 'o', 'd', 'e', 'r', ':', 'b', 'u', 'i', 'l', 't', 'i', 'n', 's', ':', 'E', 'x', 'p', 'l', 'i', 'c', 'i', 't' };
    emlrtStack c2_b_st;
    emlrtStack c2_c_st;
    emlrtStack c2_d_st;
    emlrtStack c2_st = { NULL,     /* site */
NULL,     /* tls */
NULL    /* prev */
 };
    const mxArray *c2_b_y = NULL;
    const mxArray *c2_c_y = NULL;
    const mxArray *c2_d_y = NULL;
    const mxArray *c2_e_y = NULL;
    const mxArray *c2_f_y = NULL;
    const mxArray *c2_g_y = NULL;
    const mxArray *c2_h_y = NULL;
    const mxArray *c2_i_y = NULL;
    const mxArray *c2_y = NULL;
    int32_T c2_err;
    (void)S;
    c2_st.tls = ptr_gvar_instance->c2_fEmlrtCtx;
    c2_b_st.prev = &c2_st;
    c2_b_st.tls = c2_st.tls;
    c2_c_st.prev = &c2_b_st;
    c2_c_st.tls = c2_b_st.tls;
    c2_d_st.prev = &c2_c_st;
    c2_d_st.tls = c2_c_st.tls;
    c2_b_st.site = &ptr_gvar_instance->c2_jb_emlrtRSI;
    if (!ptr_gvar_instance->c2_gobj_0.matlabCodegenIsDeleted) {
        ptr_gvar_instance->c2_gobj_0.matlabCodegenIsDeleted = true;
        c2_c_st.site = &ptr_gvar_instance->c2_hb_emlrtRSI;
        c2_d_st.site = &ptr_gvar_instance->c2_ib_emlrtRSI;
        c2_err = deleteDelaunayTriStruct(&ptr_gvar_instance->c2_gobj_0.delTri);
        if (c2_err != -1) {
        } else {
            c2_y = NULL;
            sf_mex_assign(&c2_y, sf_mex_create("y", c2_b, 10, 0U, 1, 0U, 2, 1, 23), false);
            c2_b_y = NULL;
            sf_mex_assign(&c2_b_y, sf_mex_create("y", c2_b, 10, 0U, 1, 0U, 2, 1, 23), false);
            c2_c_y = NULL;
            sf_mex_assign(&c2_c_y, sf_mex_create("y", c2_c, 10, 0U, 1, 0U, 2, 1, 52), false);
            sf_mex_call(&c2_d_st, &ptr_gvar_instance->c2_c_emlrtMCI, "error", 0U, 2U, 14, c2_y, 14, sf_mex_call(&c2_d_st, NULL, "getString", 1U, 1U, 14, sf_mex_call(&c2_d_st, NULL, "message", 1U, 2U, 14, c2_b_y, 14, c2_c_y)));
        }
        c2_err = deleteConvexHullStruct(&ptr_gvar_instance->c2_gobj_0.convHull);
        if (c2_err != -1) {
        } else {
            c2_d_y = NULL;
            sf_mex_assign(&c2_d_y, sf_mex_create("y", c2_b, 10, 0U, 1, 0U, 2, 1, 23), false);
            c2_e_y = NULL;
            sf_mex_assign(&c2_e_y, sf_mex_create("y", c2_b, 10, 0U, 1, 0U, 2, 1, 23), false);
            c2_f_y = NULL;
            sf_mex_assign(&c2_f_y, sf_mex_create("y", c2_c, 10, 0U, 1, 0U, 2, 1, 52), false);
            sf_mex_call(&c2_d_st, &ptr_gvar_instance->c2_d_emlrtMCI, "error", 0U, 2U, 14, c2_d_y, 14, sf_mex_call(&c2_d_st, NULL, "getString", 1U, 1U, 14, sf_mex_call(&c2_d_st, NULL, "message", 1U, 2U, 14, c2_e_y, 14, c2_f_y)));
        }
        c2_err = deleteDelaunayTriStruct(&ptr_gvar_instance->c2_gobj_0.convHullTri);
        if (c2_err != -1) {
        } else {
            c2_g_y = NULL;
            sf_mex_assign(&c2_g_y, sf_mex_create("y", c2_b, 10, 0U, 1, 0U, 2, 1, 23), false);
            c2_h_y = NULL;
            sf_mex_assign(&c2_h_y, sf_mex_create("y", c2_b, 10, 0U, 1, 0U, 2, 1, 23), false);
            c2_i_y = NULL;
            sf_mex_assign(&c2_i_y, sf_mex_create("y", c2_c, 10, 0U, 1, 0U, 2, 1, 52), false);
            sf_mex_call(&c2_d_st, &ptr_gvar_instance->c2_e_emlrtMCI, "error", 0U, 2U, 14, c2_g_y, 14, sf_mex_call(&c2_d_st, NULL, "getString", 1U, 1U, 14, sf_mex_call(&c2_d_st, NULL, "message", 1U, 2U, 14, c2_h_y, 14, c2_i_y)));
        }
    }
}

void mdl_setup_runtime_resources_c2_BSFC_simulink(SimStruct *S, gvar_instance *ptr_gvar_instance)
{
    sfSetAnimationVectors(S, &ptr_gvar_instance->c2_JITStateAnimation[0], &ptr_gvar_instance->c2_JITTransitionAnimation[0]);
}

void mdl_cleanup_runtime_resources_c2_BSFC_simulink(SimStruct *S, gvar_instance *ptr_gvar_instance)
{
    (void)S;
    (void)ptr_gvar_instance;
}

void enable_c2_BSFC_simulink(SimStruct *S, gvar_instance *ptr_gvar_instance)
{
    (void)ptr_gvar_instance;
    sf_get_time(S);
}

void disable_c2_BSFC_simulink(SimStruct *S, gvar_instance *ptr_gvar_instance)
{
    (void)ptr_gvar_instance;
    sf_get_time(S);
}

void sf_gateway_c2_BSFC_simulink(SimStruct *S, gvar_instance *ptr_gvar_instance)
{
    c2_coder_internal_qhullStructsWrapper *c2_d;
    emlrtStack c2_b_st;
    emlrtStack c2_c_st;
    emlrtStack c2_d_st;
    emlrtStack c2_e_st;
    emlrtStack c2_f_st;
    emlrtStack c2_g_st;
    emlrtStack c2_h_st;
    emlrtStack c2_i_st;
    emlrtStack c2_j_st;
    emlrtStack c2_k_st;
    emlrtStack c2_st = { NULL,     /* site */
NULL,     /* tls */
NULL    /* prev */
 };
    real_T c2_b_tmp_data[1342];
    real_T c2_b[671];
    real_T c2_tmp_data[671];
    real_T c2_wkspc[9];
    real_T c2_B[3];
    real_T c2_bc[3];
    real_T c2_Xq[2];
    real_T c2_qp[2];
    real_T c2_maxval;
    real_T c2_singleQueryOut;
    real_T c2_y;
    int32_T c2_vxId[3];
    int32_T c2_b_tmp_size[2];
    int32_T c2_tmp_size[1];
    int32_T c2_a__1;
    int32_T c2_r2;
    int32_T c2_r3;
    int32_T c2_rtemp;
    int32_T c2_sxId;
    int32_T exitg1;
    boolean_T c2_c;
    boolean_T c2_doExtrap;
    c2_st.tls = ptr_gvar_instance->c2_fEmlrtCtx;
    c2_b_st.prev = &c2_st;
    c2_b_st.tls = c2_st.tls;
    c2_c_st.prev = &c2_b_st;
    c2_c_st.tls = c2_b_st.tls;
    c2_d_st.prev = &c2_c_st;
    c2_d_st.tls = c2_c_st.tls;
    c2_e_st.prev = &c2_d_st;
    c2_e_st.tls = c2_d_st.tls;
    c2_f_st.prev = &c2_e_st;
    c2_f_st.tls = c2_e_st.tls;
    c2_g_st.prev = &c2_f_st;
    c2_g_st.tls = c2_f_st.tls;
    c2_h_st.prev = &c2_g_st;
    c2_h_st.tls = c2_g_st.tls;
    c2_i_st.prev = &c2_h_st;
    c2_i_st.tls = c2_h_st.tls;
    c2_j_st.prev = &c2_i_st;
    c2_j_st.tls = c2_i_st.tls;
    c2_k_st.prev = &c2_j_st;
    c2_k_st.tls = c2_j_st.tls;
    sf_get_time(S);
    ptr_gvar_instance->c2_JITTransitionAnimation[0] = 0U;
    if (!ptr_gvar_instance->c2_F_not_empty) {
        c2_b_st.site = &ptr_gvar_instance->c2_b_emlrtRSI;
        c2_scatteredInterpolant_scatteredInterpolant(S, ptr_gvar_instance, &c2_b_st, &ptr_gvar_instance->c2_gobj_0, c2_tmp_data, c2_tmp_size, &c2_r3, c2_b_tmp_data, c2_b_tmp_size, &c2_d, c2_b, &c2_doExtrap, &c2_c);
        ptr_gvar_instance->c2_F.delTri.qhWrapper = c2_d;
        ptr_gvar_instance->c2_F.validInterpolant = c2_c;
        ptr_gvar_instance->c2_F.delTri.dupesExist = c2_doExtrap;
        for (c2_sxId = 0; c2_sxId < 671; c2_sxId++) {
            ptr_gvar_instance->c2_F.delTri.idxMap[c2_sxId] = c2_b[c2_sxId];
        }
        ptr_gvar_instance->c2_F.delTri.thePoints.size[0] = 2;
        ptr_gvar_instance->c2_F.delTri.thePoints.size[1] = c2_b_tmp_size[1];
        c2_r2 = c2_b_tmp_size[0] * c2_b_tmp_size[1] - 1;
        for (c2_sxId = 0; c2_sxId <= c2_r2; c2_sxId++) {
            ptr_gvar_instance->c2_F.delTri.thePoints.data[c2_sxId] = c2_b_tmp_data[c2_sxId];
        }
        ptr_gvar_instance->c2_F.delTri.numPts = c2_r3;
        ptr_gvar_instance->c2_F.sampleVal.size[0] = c2_tmp_size[0];
        c2_r2 = c2_tmp_size[0] - 1;
        for (c2_r3 = 0; c2_r3 <= c2_r2; c2_r3++) {
            ptr_gvar_instance->c2_F.sampleVal.data[c2_r3] = c2_tmp_data[c2_r3];
        }
        ptr_gvar_instance->c2_F_not_empty = true;
        ptr_gvar_instance->c2_rpm_min = -2969.2307692307695;
        ptr_gvar_instance->c2_rpm_min_not_empty = true;
        ptr_gvar_instance->c2_rpm_max = 8203.4188034188028;
        ptr_gvar_instance->c2_rpm_max_not_empty = true;
        ptr_gvar_instance->c2_torque_min = 0.0;
        ptr_gvar_instance->c2_torque_min_not_empty = true;
        ptr_gvar_instance->c2_torque_max = 253.296703296703;
        ptr_gvar_instance->c2_torque_max_not_empty = true;
    }
    c2_b_st.site = &ptr_gvar_instance->c2_emlrtRSI;
    c2_Xq[0] = muDoubleScalarMax(-2969.2307692307695, muDoubleScalarMin(8203.4188034188028, *ptr_gvar_instance->c2_rpm));
    c2_Xq[1] = muDoubleScalarMax(0.0, muDoubleScalarMin(253.296703296703, *ptr_gvar_instance->c2_torque));
    c2_c_st.site = &ptr_gvar_instance->c2_l_emlrtRSI;
    c2_d_st.site = &ptr_gvar_instance->c2_p_emlrtRSI;
    c2_e_st.site = &ptr_gvar_instance->c2_m_emlrtRSI;
    c2_f_st.site = &ptr_gvar_instance->c2_n_emlrtRSI;
    c2_f_st.site = &ptr_gvar_instance->c2_n_emlrtRSI;
    for (c2_rtemp = 0; c2_rtemp < 2; c2_rtemp++) {
        c2_qp[c2_rtemp] = c2_Xq[c2_rtemp];
    }
    c2_d_st.site = &ptr_gvar_instance->c2_o_emlrtRSI;
    c2_doExtrap = true;
    c2_e_st.site = &ptr_gvar_instance->c2_q_emlrtRSI;
    c2_f_st.site = &ptr_gvar_instance->c2_v_emlrtRSI;
    c2_sxId = tsearch(&ptr_gvar_instance->c2_F.delTri.qhWrapper->delTri, &c2_qp[0]);
    if (c2_sxId != -1) {
        c2_e_st.site = &ptr_gvar_instance->c2_r_emlrtRSI;
        c2_f_st.site = &ptr_gvar_instance->c2_w_emlrtRSI;
        getVtxIDsOfSimplex(&ptr_gvar_instance->c2_F.delTri.qhWrapper->delTri, c2_sxId, &c2_vxId[0]);
        for (c2_rtemp = 0; c2_rtemp < 3; c2_rtemp++) {
            c2_e_st.site = &ptr_gvar_instance->c2_s_emlrtRSI;
            for (c2_r3 = 0; c2_r3 < 2; c2_r3++) {
                c2_wkspc[c2_rtemp + 3 * c2_r3] = ptr_gvar_instance->c2_F.delTri.thePoints.data[c2_r3 + ((c2_vxId[c2_rtemp] - 1) << 1)];
            }
            c2_wkspc[c2_rtemp + 6] = 1.0;
        }
        c2_e_st.site = &ptr_gvar_instance->c2_t_emlrtRSI;
        for (c2_r3 = 0; c2_r3 < 2; c2_r3++) {
            c2_bc[c2_r3] = c2_qp[c2_r3] * 0.5;
        }
        c2_bc[2] = 0.5;
        c2_rtemp = 0;
        do {
            exitg1 = 0;
            if (c2_rtemp < 3) {
                c2_sxId = 1;
                for (c2_r2 = 0; c2_r2 < 2; c2_r2++) {
                    c2_wkspc[c2_rtemp + 3 * c2_r2] *= 0.5;
                    if (c2_sxId != 0) {
                        c2_sxId = (int32_T)(c2_wkspc[c2_rtemp + 3 * c2_r2] == c2_bc[c2_r2]);
                    }
                    c2_wkspc[c2_rtemp + 3 * c2_r2] -= c2_bc[c2_r2];
                }
                c2_wkspc[c2_rtemp + 6] *= 0.5;
                if (c2_sxId != 0) {
                    memset(&c2_bc[0], 0, 3U * sizeof(real_T));
                    c2_sxId = c2_rtemp + 1;
                    c2_doExtrap = false;
                    c2_singleQueryOut = ptr_gvar_instance->c2_F.sampleVal.data[c2_vxId[c2_sxId - 1] - 1];
                    exitg1 = 1;
                } else {
                    c2_rtemp++;
                }
            } else {
                memset(&c2_bc[0], 0, sizeof(real_T) << 1);
                c2_f_st.site = &ptr_gvar_instance->c2_x_emlrtRSI;
                for (c2_r3 = 0; c2_r3 < 3; c2_r3++) {
                    c2_B[c2_r3] = c2_bc[c2_r3];
                }
                c2_g_st.site = &ptr_gvar_instance->c2_y_emlrtRSI;
                c2_h_st.site = &ptr_gvar_instance->c2_ab_emlrtRSI;
                c2_i_st.site = &ptr_gvar_instance->c2_bb_emlrtRSI;
                c2_sxId = 0;
                c2_r2 = 1;
                c2_r3 = 2;
                c2_maxval = muDoubleScalarAbs(c2_wkspc[0]);
                c2_y = muDoubleScalarAbs(c2_wkspc[1]);
                if (c2_y > muDoubleScalarAbs(c2_wkspc[0])) {
                    c2_maxval = c2_y;
                    c2_sxId = 1;
                    c2_r2 = 0;
                }
                if (muDoubleScalarAbs(c2_wkspc[2]) > c2_maxval) {
                    c2_sxId = 2;
                    c2_r2 = 1;
                    c2_r3 = 0;
                }
                c2_wkspc[c2_r2] /= c2_wkspc[c2_sxId];
                c2_wkspc[c2_r3] /= c2_wkspc[c2_sxId];
                c2_wkspc[c2_r2 + 3] -= c2_wkspc[c2_r2] * c2_wkspc[c2_sxId + 3];
                c2_wkspc[c2_r3 + 3] -= c2_wkspc[c2_r3] * c2_wkspc[c2_sxId + 3];
                c2_wkspc[c2_r2 + 6] -= c2_wkspc[c2_r2] * c2_wkspc[c2_sxId + 6];
                c2_wkspc[c2_r3 + 6] -= c2_wkspc[c2_r3] * c2_wkspc[c2_sxId + 6];
                if (muDoubleScalarAbs(c2_wkspc[c2_r3 + 3]) > muDoubleScalarAbs(c2_wkspc[c2_r2 + 3])) {
                    c2_rtemp = c2_r2;
                    c2_r2 = c2_r3;
                    c2_r3 = c2_rtemp;
                }
                c2_wkspc[c2_r3 + 3] /= c2_wkspc[c2_r2 + 3];
                c2_wkspc[c2_r3 + 6] -= c2_wkspc[c2_r3 + 3] * c2_wkspc[c2_r2 + 6];
                if ((c2_wkspc[c2_sxId] == 0.0) || (c2_wkspc[c2_r2 + 3] == 0.0) || (c2_wkspc[c2_r3 + 6] == 0.0)) {
                    c2_j_st.site = &ptr_gvar_instance->c2_cb_emlrtRSI;
                    c2_k_st.site = &ptr_gvar_instance->c2_db_emlrtRSI;
                    c2_b_warning(S, ptr_gvar_instance, &c2_k_st);
                }
                c2_bc[c2_sxId] = c2_B[0] / c2_wkspc[c2_sxId];
                c2_bc[c2_r2] = c2_B[1] - c2_bc[c2_sxId] * c2_wkspc[c2_sxId + 3];
                c2_bc[c2_r3] = c2_B[2] - c2_bc[c2_sxId] * c2_wkspc[c2_sxId + 6];
                c2_bc[c2_r2] /= c2_wkspc[c2_r2 + 3];
                c2_bc[c2_r3] -= c2_bc[c2_r2] * c2_wkspc[c2_r2 + 6];
                c2_bc[c2_r3] /= c2_wkspc[c2_r3 + 6];
                c2_bc[c2_r2] -= c2_bc[c2_r3] * c2_wkspc[c2_r3 + 3];
                c2_bc[c2_sxId] -= c2_bc[c2_r3] * c2_wkspc[c2_r3];
                c2_bc[c2_sxId] -= c2_bc[c2_r2] * c2_wkspc[c2_r2];
                c2_doExtrap = true;
                for (c2_sxId = 0; c2_sxId < 3; c2_sxId++) {
                    if (c2_doExtrap && ((!muDoubleScalarIsInf(c2_bc[c2_sxId])) && (!muDoubleScalarIsNaN(c2_bc[c2_sxId])))) {
                    } else {
                        c2_doExtrap = false;
                    }
                }
                c2_doExtrap = !c2_doExtrap;
                if (!c2_doExtrap) {
                    c2_singleQueryOut = 0.0;
                    for (c2_rtemp = 0; c2_rtemp < 3; c2_rtemp++) {
                        c2_singleQueryOut += c2_bc[c2_rtemp] * ptr_gvar_instance->c2_F.sampleVal.data[c2_vxId[c2_rtemp] - 1];
                    }
                }
                exitg1 = 1;
            }
        } while (exitg1 == 0);
    }
    if (c2_doExtrap) {
        c2_e_st.site = &ptr_gvar_instance->c2_u_emlrtRSI;
        c2_f_st.site = &ptr_gvar_instance->c2_eb_emlrtRSI;
        c2_g_st.site = &ptr_gvar_instance->c2_p_emlrtRSI;
        c2_h_st.site = &ptr_gvar_instance->c2_fb_emlrtRSI;
        c2_i_st.site = &ptr_gvar_instance->c2_gb_emlrtRSI;
        c2_sxId = dsearch(&ptr_gvar_instance->c2_F.delTri.qhWrapper->delTri, &c2_qp[0], &c2_a__1);
        c2_singleQueryOut = ptr_gvar_instance->c2_F.sampleVal.data[c2_sxId - 1];
    }
    *ptr_gvar_instance->c2_bsfc = c2_singleQueryOut;
}

void ext_mode_exec_c2_BSFC_simulink(SimStruct *S, gvar_instance *ptr_gvar_instance)
{
    (void)S;
    (void)ptr_gvar_instance;
}

const mxArray *get_sim_state_c2_BSFC_simulink(SimStruct *S, gvar_instance *ptr_gvar_instance)
{
    const mxArray *c2_b_y = NULL;
    const mxArray *c2_c_y = NULL;
    const mxArray *c2_d_y = NULL;
    const mxArray *c2_e_y = NULL;
    const mxArray *c2_f_y = NULL;
    const mxArray *c2_st;
    const mxArray *c2_y = NULL;
    (void)S;
    c2_st = NULL;
    c2_y = NULL;
    sf_mex_assign(&c2_y, sf_mex_createcellmatrix(5, 1), false);
    c2_b_y = NULL;
    sf_mex_assign(&c2_b_y, sf_mex_create("y", ptr_gvar_instance->c2_bsfc, 0, 0U, 0, 0U, 0), false);
    sf_mex_setcell(c2_y, 0, c2_b_y);
    c2_c_y = NULL;
    if (!ptr_gvar_instance->c2_rpm_max_not_empty) {
        sf_mex_assign(&c2_c_y, sf_mex_create("y", NULL, 0, 0U, 1, 0U, 2, 0, 0), false);
    } else {
        sf_mex_assign(&c2_c_y, sf_mex_create("y", &ptr_gvar_instance->c2_rpm_max, 0, 0U, 0, 0U, 0), false);
    }
    sf_mex_setcell(c2_y, 1, c2_c_y);
    c2_d_y = NULL;
    if (!ptr_gvar_instance->c2_rpm_max_not_empty) {
        sf_mex_assign(&c2_d_y, sf_mex_create("y", NULL, 0, 0U, 1, 0U, 2, 0, 0), false);
    } else {
        sf_mex_assign(&c2_d_y, sf_mex_create("y", &ptr_gvar_instance->c2_rpm_min, 0, 0U, 0, 0U, 0), false);
    }
    sf_mex_setcell(c2_y, 2, c2_d_y);
    c2_e_y = NULL;
    if (!ptr_gvar_instance->c2_rpm_max_not_empty) {
        sf_mex_assign(&c2_e_y, sf_mex_create("y", NULL, 0, 0U, 1, 0U, 2, 0, 0), false);
    } else {
        sf_mex_assign(&c2_e_y, sf_mex_create("y", &ptr_gvar_instance->c2_torque_max, 0, 0U, 0, 0U, 0), false);
    }
    sf_mex_setcell(c2_y, 3, c2_e_y);
    c2_f_y = NULL;
    if (!ptr_gvar_instance->c2_rpm_max_not_empty) {
        sf_mex_assign(&c2_f_y, sf_mex_create("y", NULL, 0, 0U, 1, 0U, 2, 0, 0), false);
    } else {
        sf_mex_assign(&c2_f_y, sf_mex_create("y", &ptr_gvar_instance->c2_torque_min, 0, 0U, 0, 0U, 0), false);
    }
    sf_mex_setcell(c2_y, 4, c2_f_y);
    sf_mex_assign(&c2_st, c2_y, false);
    return c2_st;
}

void set_sim_state_c2_BSFC_simulink(SimStruct *S, gvar_instance *ptr_gvar_instance, const mxArray *c2_st)
{
    const mxArray *c2_u;
    c2_u = sf_mex_dup(c2_st);
    *ptr_gvar_instance->c2_bsfc = c2_emlrt_marshallIn(S, ptr_gvar_instance, sf_mex_dup(sf_mex_getcell(c2_u, 0)), "bsfc");
    ptr_gvar_instance->c2_rpm_max = c2_c_emlrt_marshallIn(S, ptr_gvar_instance, sf_mex_dup(sf_mex_getcell(c2_u, 1)), "rpm_max", &ptr_gvar_instance->c2_rpm_max_not_empty);
    ptr_gvar_instance->c2_rpm_min = c2_c_emlrt_marshallIn(S, ptr_gvar_instance, sf_mex_dup(sf_mex_getcell(c2_u, 2)), "rpm_min", &ptr_gvar_instance->c2_rpm_min_not_empty);
    ptr_gvar_instance->c2_torque_max = c2_c_emlrt_marshallIn(S, ptr_gvar_instance, sf_mex_dup(sf_mex_getcell(c2_u, 3)), "torque_max", &ptr_gvar_instance->c2_torque_max_not_empty);
    ptr_gvar_instance->c2_torque_min = c2_c_emlrt_marshallIn(S, ptr_gvar_instance, sf_mex_dup(sf_mex_getcell(c2_u, 4)), "torque_min", &ptr_gvar_instance->c2_torque_min_not_empty);
    sf_mex_destroy(&c2_u);
    sf_mex_destroy(&c2_st);
}

boolean_T c2_sortLE(SimStruct *S, gvar_instance *ptr_gvar_instance, real_T c2_v[1342], int32_T c2_idx1, int32_T c2_idx2)
{
    int32_T c2_k;
    boolean_T c2_p;
    boolean_T exitg1;
    (void)S;
    (void)ptr_gvar_instance;
    c2_p = true;
    c2_k = 0;
    exitg1 = false;
    while ((!exitg1) && (c2_k < 2)) {
        if (c2_v[(c2_idx1 + 671 * c2_k) - 1] == c2_v[(c2_idx2 + 671 * c2_k) - 1]) {
            c2_k++;
        } else {
            if (c2_v[(c2_idx1 + 671 * c2_k) - 1] <= c2_v[(c2_idx2 + 671 * c2_k) - 1]) {
            } else {
                c2_p = false;
            }
            exitg1 = true;
        }
    }
    return c2_p;
}

void c2_scatteredInterpolant_scatteredInterpolant(SimStruct *S, gvar_instance *ptr_gvar_instance, const emlrtStack *c2_sp, c2_coder_internal_qhullStructsWrapper *c2_iobj_0, real_T c2_obj_sampleVal_data[], int32_T c2_obj_sampleVal_size[1], int32_T *c2_obj_delTri_numPts, real_T c2_obj_delTri_thePoints_data[], int32_T c2_obj_delTri_thePoints_size[2], c2_coder_internal_qhullStructsWrapper **c2_obj_delTri_qhWrapper, real_T c2_obj_delTri_idxMap[671], boolean_T *c2_obj_delTri_dupesExist, boolean_T *c2_obj_validInterpolant)
{
    static real_T c2_c[1342] = { 1926.4523076923, 1570.94017094017, 1338.46153846153, 1735.04273504273, 2158.97435897435, 2459.82905982906, 2664.95726495726, 2213.67521367521, 1830.76923076923, 1570.94017094017, 1338.46153846153, 1735.04273504273, 2158.97435897435, 2459.82905982906, 2664.95726495726, 2213.67521367521, 1830.76923076923, 1830.7692307692305, 1283.76068376068, 982.90598290598223, 1488.8888888888814, 2131.6239316239271, 2897.4358974358952, 2993.1623931623917, 2350.4273504273456, 982.90598290598223, 1488.8888888888814, 2131.6239316239271, 2897.4358974358952, 2993.1623931623917, 2350.4273504273456, 1830.7692307692305, 1283.76068376068, 1475.2136752136728, 2186.32478632478, 2993.1623931623917, 3553.84615384615, 3512.8205128205063, 3211.9658119658093, 2801.7094017093991, 2555.5555555555547, 2104.2735042735012, 1707.6923076922997, 1242.7350427350432, 846.15384615384608, 600.0, 463.24786324786385, 818.803418803419, 1229.0598290598291, 846.15384615384608, 600.0, 463.24786324786385, 818.803418803419, 1229.0598290598291, 1475.2136752136728, 2186.32478632478, 2993.1623931623917, 3553.84615384615, 3512.8205128205063, 3211.9658119658093, 2801.7094017093991, 2555.5555555555547, 2104.2735042735012, 1707.6923076922997, 1242.7350427350432, 1256.4102564102564, 818.803418803419, 326.49572649572684, 148.71794871794884, 312.82051282051287, 805.12820512820508, 1037.6068376068372, 1762.3931623931608, 3034.188034188026, 3608.547008547002, 3868.3760683760638, 4100.8547008547, 3977.7777777777774, 3950.4273504273428, 3540.1709401709322, 3006.8376068376, 2788.0341880341816, 2569.2307692307636, 2131.6239316239271, 1694.0170940170908, 148.71794871794884, 312.82051282051287, 805.12820512820508, 1037.6068376068372, 1762.3931623931608, 3034.188034188026, 3608.547008547002, 3868.3760683760638, 4100.8547008547, 3977.7777777777774, 3950.4273504273428, 3540.1709401709322, 3006.8376068376, 2788.0341880341816, 2569.2307692307636, 2131.6239316239271, 1694.0170940170908, 1256.4102564102564, 818.803418803419, 326.49572649572684, 818.803418803419, 545.29914529914561, 94.017094017094678, -165.81196581196536, -1.709401709401277, 340.17094017094081, 818.803418803419, 1133.3333333333333, 1830.7692307692305, 2254.700854700849, 2788.0341880341816, 3635.8974358974283, 4114.5299145299086, 4538.4615384615354, 4401.7094017093959, 4087.1794871794823, 3799.9999999999941, 3225.6410256410181, 2829.0598290598255, 2405.1282051281978, 2104.2735042735012, 1666.6666666666647, 1461.5384615384614, 1037.6068376068372, -165.81196581196536, -1.709401709401277, 340.17094017094081, 818.803418803419, 1133.3333333333333, 1830.7692307692305, 2254.700854700849, 2788.0341880341816, 3635.8974358974283, 4114.5299145299086, 4538.4615384615354, 4401.7094017093959, 4087.1794871794823, 3799.9999999999941, 3225.6410256410181, 2829.0598290598255, 2405.1282051281978, 2104.2735042735012, 1666.6666666666647, 1461.5384615384614, 1037.6068376068372, 818.803418803419, 545.29914529914561, 94.017094017094678, 3635.8974358974283, 4210.2564102564047, 4483.7606837606836, 4743.5897435897368, 4729.9145299145284, 4620.5128205128149, 4210.2564102564047, 3882.0512820512813, 3335.0427350427317, 3088.8888888888869, 2405.1282051281978, 2035.8974358974313, 1912.8205128205091, 1639.3162393162386, 1119.6581196581192, 613.67521367521385, -1.709401709401277, -357.26495726495125, -411.96581196580371, -370.94017094016863, -97.435897435897346, 367.52136752136789, 805.12820512820508, 1037.6068376068372, 1283.7606837606834, 1871.7948717948655, 2295.7264957264933, 2623.9316239316158, 3047.8632478632435, -411.96581196580371, -370.94017094016863, -97.435897435897346, 367.52136752136789, 805.12820512820508, 1037.6068376068372, 1283.7606837606834, 1871.7948717948655, 2295.7264957264933, 2623.9316239316158, 3047.8632478632435, 3635.8974358974283, 4210.2564102564047, 4483.7606837606836, 4743.5897435897368, 4729.9145299145284, 4620.5128205128149, 4210.2564102564047, 3882.0512820512813, 3335.0427350427317, 3088.8888888888869, 2405.1282051281978, 2035.8974358974313, 1912.8205128205091, 1639.3162393162386, 1119.6581196581192, 613.67521367521385, -1.709401709401277, -357.26495726495125, 1037.6068376068372, 1475.2136752136728, 2117.9487179487187, 2829.0598290598255, 3198.2905982905918, 3690.5982905982892, 4524.7863247863188, 4798.2905982905977, 5085.4700854700859, 5181.1965811965729, 5495.7264957264952, 5413.6752136752084, 5290.5982905982864, 4333.3333333333267, 3717.9487179487155, 3581.1965811965761, 3088.8888888888869, 2596.58119658119, 2076.9230769230753, 1666.6666666666647, 1270.0854700854693, 996.5811965811962, 627.35042735042771, 162.39316239316275, -452.9914529914472, -630.76923076923049, -548.71794871794327, -220.51282051282033, 312.82051282051287, 777.777777777778, -630.76923076923049, -548.71794871794327, -220.51282051282033, 312.82051282051287, 777.777777777778, 1037.6068376068372, 1475.2136752136728, 2117.9487179487187, 2829.0598290598255, 3198.2905982905918, 3690.5982905982892, 4524.7863247863188, 4798.2905982905977, 5085.4700854700859, 5181.1965811965729, 5495.7264957264952, 5413.6752136752084, 5290.5982905982864, 4333.3333333333267, 3717.9487179487155, 3581.1965811965761, 3088.8888888888869, 2596.58119658119, 2076.9230769230753, 1666.6666666666647, 1270.0854700854693, 996.5811965811962, 627.35042735042771, 162.39316239316275, -452.9914529914472, 5099.1452991452943, 5878.6324786324712, 6152.13675213675, 6138.4615384615336, 5851.2820512820463, 5386.3247863247834, 4675.2136752136666, 4182.9059829059788, 3882.0512820512813, 2952.1367521367479, 1830.7692307692305, 1064.9572649572642, 600.0, -535.04273504273465, -849.572649572649, -740.17094017093541, -288.88888888888141, 244.4444444444448, 600.0, 1010.2564102564102, 1830.7692307692305, 2705.9829059829035, 3280.3418803418795, 4100.8547008547, 4305.9829059829008, 4565.8119658119622, -740.17094017093541, -288.88888888888141, 244.4444444444448, 600.0, 1010.2564102564102, 1830.7692307692305, 2705.9829059829035, 3280.3418803418795, 4100.8547008547, 4305.9829059829008, 4565.8119658119622, 5099.1452991452943, 5878.6324786324712, 6152.13675213675, 6138.4615384615336, 5851.2820512820463, 5386.3247863247834, 4675.2136752136666, 4182.9059829059788, 3882.0512820512813, 2952.1367521367479, 1830.7692307692305, 1064.9572649572642, 600.0, -535.04273504273465, -849.572649572649, 600.0, 1735.0427350427346, 2883.7606837606777, 3964.10256410256, 4770.9401709401718, 5413.6752136752084, 5851.2820512820463, 6370.9401709401682, 6685.4700854700832, 6808.5470085470051, 6589.7435897435862, 5823.9316239316186, 4948.7179487179455, 3799.9999999999941, 3225.6410256410181, 2514.5299145299114, 1830.7692307692305, 736.752136752137, -247.86324786324747, -1136.7521367521279, -1437.6068376068336, -1423.9316239316158, -945.29914529914458, -425.64102564102109, 80.34188034188071, -1423.9316239316158, -945.29914529914458, -425.64102564102109, 80.34188034188071, 600.0, 1735.0427350427346, 2883.7606837606777, 3964.10256410256, 4770.9401709401718, 5413.6752136752084, 5851.2820512820463, 6370.9401709401682, 6685.4700854700832, 6808.5470085470051, 6589.7435897435862, 5823.9316239316186, 4948.7179487179455, 3799.9999999999941, 3225.6410256410181, 2514.5299145299114, 1830.7692307692305, 736.752136752137, -247.86324786324747, -1136.7521367521279, -1437.6068376068336, 600.0, 1967.5213675213613, 3239.3162393162356, 4716.2393162393109, 5400.0, 5974.3589743589673, 6603.4188034187964, 6835.897435897431, 7013.6752136752148, 6781.1965811965783, 6343.5897435897432, 6015.3846153846107, 5755.5555555555484, 4770.9401709401718, 3964.10256410256, 3294.0170940170879, 2487.1794871794846, 1694.0170940170908, 791.45299145299111, 449.57264957264988, -439.31623931623858, -1314.5299145299111, -1588.0341880341816, -1588.0341880341816, -1423.9316239316158, -1068.3760683760668, -658.11965811965661, 25.641025641026658, -1588.0341880341816, -1423.9316239316158, -1068.3760683760668, -658.11965811965661, 25.641025641026658, 600.0, 1967.5213675213613, 3239.3162393162356, 4716.2393162393109, 5400.0, 5974.3589743589673, 6603.4188034187964, 6835.897435897431, 7013.6752136752148, 6781.1965811965783, 6343.5897435897432, 6015.3846153846107, 5755.5555555555484, 4770.9401709401718, 3964.10256410256, 3294.0170940170879, 2487.1794871794846, 1694.0170940170908, 791.45299145299111, 449.57264957264988, -439.31623931623858, -1314.5299145299111, -1588.0341880341816, 5400.0, 6070.0854700854634, 7041.0256410256407, 7410.2564102564074, 6972.6495726495705, 6261.5384615384546, 4866.666666666667, 3430.7692307692273, 2282.0512820512754, 1174.3589743589741, -179.48717948717933, -1068.3760683760668, -1683.7606837606777, -1847.8632478632435, -1888.8888888888869, -1560.6837606837553, -863.24786324785759, -1.709401709401277, 572.64957264957263, 2528.2051282051289, 3280.3418803418795, 4511.1111111111095, -1888.8888888888869, -1560.6837606837553, -863.24786324785759, -1.709401709401277, 572.64957264957263, 2528.2051282051289, 3280.3418803418795, 4511.1111111111095, 5400.0, 6070.0854700854634, 7041.0256410256407, 7410.2564102564074, 6972.6495726495705, 6261.5384615384546, 4866.666666666667, 3430.7692307692273, 2282.0512820512754, 1174.3589743589741, -179.48717948717933, -1068.3760683760668, -1683.7606837606777, -1847.8632478632435, 586.32478632478694, 2022.2222222222226, 3225.6410256410181, 5386.3247863247834, 5960.6837606837589, 7109.40170940171, 7547.0085470085478, 7683.7606837606781, 7437.6068376068333, 6917.9487179487178, 6152.13675213675, 4962.3931623931549, 3895.72649572649, 2678.6324786324772, 1598.290598290595, -398.29059829059486, -1478.6324786324772, -2217.0940170940103, -2271.7948717948716, -2011.9658119658093, -1464.9572649572597, -740.17094017093541, 189.74358974359069, -2271.7948717948716, -2011.9658119658093, -1464.9572649572597, -740.17094017093541, 189.74358974359069, 586.32478632478694, 2022.2222222222226, 3225.6410256410181, 5386.3247863247834, 5960.6837606837589, 7109.40170940171, 7547.0085470085478, 7683.7606837606781, 7437.6068376068333, 6917.9487179487178, 6152.13675213675, 4962.3931623931549, 3895.72649572649, 2678.6324786324772, 1598.290598290595, -398.29059829059486, -1478.6324786324772, -2217.0940170940103, 5400.0, 6124.7863247863243, 7013.6752136752148, 7738.461538461539, 7820.5128205128176, 7519.65811965812, 7382.9059829059806, 6589.7435897435862, 5714.5299145299141, 4729.9145299145284, 3649.5726495726458, 2801.7094017093991, 2213.6752136752057, 1174.3589743589741, -193.16239316239239, -1232.4786324786241, -2025.6410256410181, -2435.8974358974283, -2545.2991452991419, -2381.1965811965761, -1957.264957264957, -1492.307692307686, -781.19658119657879, -206.83760683760647, 600.0, 1967.5213675213613, 3252.9914529914445, -2545.2991452991419, -2381.1965811965761, -1957.264957264957, -1492.307692307686, -781.19658119657879, -206.83760683760647, 600.0, 1967.5213675213613, 3252.9914529914445, 5400.0, 6124.7863247863243, 7013.6752136752148, 7738.461538461539, 7820.5128205128176, 7519.65811965812, 7382.9059829059806, 6589.7435897435862, 5714.5299145299141, 4729.9145299145284, 3649.5726495726458, 2801.7094017093991, 2213.6752136752057, 1174.3589743589741, -193.16239316239239, -1232.4786324786241, -2025.6410256410181, -2435.8974358974283, 613.67521367521385, 1270.0854700854693, 1434.1880341880344, 1844.4444444444393, 2637.6068376068333, 3266.6666666666615, 5263.2478632478606, 5427.3504273504259, 5687.1794871794882, 6917.9487179487178, 7806.8376068376092, 8094.0170940170865, 8176.0683760683751, 7984.615384615382, 7875.2136752136694, 7246.1538461538412, 6562.39316239316, 5577.7777777777737, 4880.3418803418763, 4210.2564102564047, 3471.7948717948716, 2528.2051282051289, 1680.3418803418733, 217.09401709401772, -931.62393162392732, -1533.3333333333294, -2353.84615384615, -2695.72649572649, -2791.4529914529862, -2572.6495726495677, -2107.6923076923053, -1615.384615384608, -917.9487179487096, -111.1111111111104, -2791.4529914529862, -2572.6495726495677, -2107.6923076923053, -1615.384615384608, -917.9487179487096, -111.1111111111104, 613.67521367521385, 1270.0854700854693, 1434.1880341880344, 1844.4444444444393, 2637.6068376068333, 3266.6666666666615, 5263.2478632478606, 5427.3504273504259, 5687.1794871794882, 6917.9487179487178, 7806.8376068376092, 8094.0170940170865, 8176.0683760683751, 7984.615384615382, 7875.2136752136694, 7246.1538461538412, 6562.39316239316, 5577.7777777777737, 4880.3418803418763, 4210.2564102564047, 3471.7948717948716, 2528.2051282051289, 1680.3418803418733, 217.09401709401772, -931.62393162392732, -1533.3333333333294, -2353.84615384615, -2695.72649572649, 5441.0256410256343, 6070.0854700854634, 6726.4957264957266, 7205.1282051282069, 7765.8119658119649, 8203.4188034188028, 8176.0683760683751, 7341.8803418803382, 6070.0854700854634, 4866.666666666667, 4264.9572649572656, 1694.0170940170908, 1037.6068376068372, 162.39316239316275, -576.06837606836962, -1054.7008547008493, -1629.0598290598255, -2135.0427350427317, -2723.0769230769165, -2969.2307692307695, -2969.2307692307695, -2791.4529914529862, -2435.8974358974283, -1943.5897435897396, -1396.5811965811897, -658.11965811965661, 586.32478632478694, 1762.3931623931608, 3294.0170940170879, 4675.2136752136666, 99.4505494505494, 100.54945054945, 93.4065934065934, 91.2087912087912, 95.6043956043956, 102.197802197802, 108.791208791208, 109.890109890109, 105.494505494505, 100.54945054945, 93.4065934065934, 91.2087912087912, 95.6043956043956, 102.197802197802, 108.791208791208, 109.890109890109, 105.494505494505, 110.989010989011, 102.197802197802, 89.5604395604395, 84.065934065934, 90.1098901098901, 98.9010989010989, 109.890109890109, 114.285714285714, 89.5604395604395, 84.065934065934, 90.1098901098901, 98.9010989010989, 109.890109890109, 114.285714285714, 110.989010989011, 102.197802197802, 76.3736263736263, 80.2197802197802, 87.9120879120879, 100.0, 109.890109890109, 115.934065934065, 119.780219780219, 124.725274725274, 119.780219780219, 118.681318681318, 109.890109890109, 98.3516483516483, 94.5054945054945, 87.9120879120879, 80.2197802197802, 78.021978021978, 98.3516483516483, 94.5054945054945, 87.9120879120879, 80.2197802197802, 78.021978021978, 76.3736263736263, 80.2197802197802, 87.9120879120879, 100.0, 109.890109890109, 115.934065934065, 119.780219780219, 124.725274725274, 119.780219780219, 118.681318681318, 109.890109890109, 119.780219780219, 109.890109890109, 105.494505494505, 91.7582417582417, 79.1208791208791, 70.3296703296703, 69.7802197802197, 70.3296703296703, 79.1208791208791, 87.9120879120879, 89.5604395604395, 100.0, 109.890109890109, 119.780219780219, 129.120879120879, 134.065934065934, 139.560439560439, 139.560439560439, 134.065934065934, 129.670329670329, 91.7582417582417, 79.1208791208791, 70.3296703296703, 69.7802197802197, 70.3296703296703, 79.1208791208791, 87.9120879120879, 89.5604395604395, 100.0, 109.890109890109, 119.780219780219, 129.120879120879, 134.065934065934, 139.560439560439, 139.560439560439, 134.065934065934, 129.670329670329, 119.780219780219, 109.890109890109, 105.494505494505, 117.582417582417, 115.384615384615, 109.890109890109, 100.0, 79.6703296703296, 72.5274725274725, 65.3846153846153, 65.9340659340659, 65.9340659340659, 67.5824175824175, 70.8791208791208, 79.6703296703296, 86.8131868131868, 99.4505494505494, 114.285714285714, 129.670329670329, 136.813186813186, 141.758241758241, 153.846153846153, 153.846153846153, 141.208791208791, 135.164835164835, 129.670329670329, 123.626373626373, 100.0, 79.6703296703296, 72.5274725274725, 65.3846153846153, 65.9340659340659, 65.9340659340659, 67.5824175824175, 70.8791208791208, 79.6703296703296, 86.8131868131868, 99.4505494505494, 114.285714285714, 129.670329670329, 136.813186813186, 141.758241758241, 153.846153846153, 153.846153846153, 141.208791208791, 135.164835164835, 129.670329670329, 123.626373626373, 117.582417582417, 115.384615384615, 109.890109890109, 74.7252747252747, 79.6703296703296, 89.5604395604395, 105.494505494505, 121.428571428571, 130.21978021978, 140.10989010989, 143.956043956043, 149.450549450549, 159.340659340659, 162.087912087912, 153.296703296703, 147.252747252747, 144.505494505494, 130.76923076923, 123.626373626373, 118.681318681318, 107.692307692307, 98.9010989010989, 87.9120879120879, 74.7252747252747, 64.8351648351648, 62.087912087912, 62.087912087912, 61.5384615384615, 61.5384615384615, 63.1868131868132, 64.2857142857142, 67.5824175824175, 98.9010989010989, 87.9120879120879, 74.7252747252747, 64.8351648351648, 62.087912087912, 62.087912087912, 61.5384615384615, 61.5384615384615, 63.1868131868132, 64.2857142857142, 67.5824175824175, 74.7252747252747, 79.6703296703296, 89.5604395604395, 105.494505494505, 121.428571428571, 130.21978021978, 140.10989010989, 143.956043956043, 149.450549450549, 159.340659340659, 162.087912087912, 153.296703296703, 147.252747252747, 144.505494505494, 130.76923076923, 123.626373626373, 118.681318681318, 107.692307692307, 58.2417582417582, 57.1428571428571, 58.2417582417582, 60.4395604395604, 60.989010989011, 70.3296703296703, 77.4725274725274, 86.2637362637362, 90.1098901098901, 100.0, 108.791208791208, 128.571428571428, 135.714285714285, 145.054945054945, 153.296703296703, 160.98901098901, 168.131868131868, 168.681318681318, 165.934065934065, 162.087912087912, 151.648351648351, 146.153846153846, 135.164835164835, 128.571428571428, 117.582417582417, 98.9010989010989, 82.9670329670329, 71.4285714285714, 62.087912087912, 58.2417582417582, 98.9010989010989, 82.9670329670329, 71.4285714285714, 62.087912087912, 58.2417582417582, 58.2417582417582, 57.1428571428571, 58.2417582417582, 60.4395604395604, 60.989010989011, 70.3296703296703, 77.4725274725274, 86.2637362637362, 90.1098901098901, 100.0, 108.791208791208, 128.571428571428, 135.714285714285, 145.054945054945, 153.296703296703, 160.98901098901, 168.131868131868, 168.681318681318, 165.934065934065, 162.087912087912, 151.648351648351, 146.153846153846, 135.164835164835, 128.571428571428, 117.582417582417, 81.8681318681318, 91.2087912087912, 104.395604395604, 123.626373626373, 136.263736263736, 143.406593406593, 149.450549450549, 155.494505494505, 167.582417582417, 178.571428571428, 174.175824175824, 160.98901098901, 151.098901098901, 132.967032967032, 111.538461538461, 80.2197802197802, 66.4835164835164, 57.1428571428571, 54.9450549450549, 53.8461538461538, 52.7472527472527, 54.9450549450549, 57.1428571428571, 59.8901098901099, 69.7802197802197, 71.4285714285714, 80.2197802197802, 66.4835164835164, 57.1428571428571, 54.9450549450549, 53.8461538461538, 52.7472527472527, 54.9450549450549, 57.1428571428571, 59.8901098901099, 69.7802197802197, 71.4285714285714, 81.8681318681318, 91.2087912087912, 104.395604395604, 123.626373626373, 136.263736263736, 143.406593406593, 149.450549450549, 155.494505494505, 167.582417582417, 178.571428571428, 174.175824175824, 160.98901098901, 151.098901098901, 132.967032967032, 111.538461538461, 41.7582417582417, 42.8571428571428, 44.5054945054945, 47.2527472527472, 49.4505494505494, 53.2967032967033, 63.1868131868132, 74.7252747252747, 93.9560439560439, 120.32967032967, 147.252747252747, 164.835164835164, 179.120879120879, 192.857142857142, 200.0, 198.901098901098, 193.406593406593, 180.76923076923, 165.934065934065, 146.153846153846, 122.527472527472, 89.5604395604395, 62.6373626373626, 51.6483516483516, 43.4065934065934, 89.5604395604395, 62.6373626373626, 51.6483516483516, 43.4065934065934, 41.7582417582417, 42.8571428571428, 44.5054945054945, 47.2527472527472, 49.4505494505494, 53.2967032967033, 63.1868131868132, 74.7252747252747, 93.9560439560439, 120.32967032967, 147.252747252747, 164.835164835164, 179.120879120879, 192.857142857142, 200.0, 198.901098901098, 193.406593406593, 180.76923076923, 165.934065934065, 146.153846153846, 122.527472527472, 38.4615384615384, 39.0109890109889, 39.0109890109889, 43.9560439560439, 46.1538461538461, 60.4395604395604, 76.9230769230769, 90.6593406593406, 120.87912087912, 152.747252747252, 167.032967032967, 169.230769230769, 174.725274725274, 190.10989010989, 200.0, 208.791208791208, 208.241758241758, 201.648351648351, 190.10989010989, 186.813186813186, 171.978021978021, 149.450549450549, 126.373626373626, 92.3076923076922, 76.9230769230769, 60.989010989011, 50.5494505494505, 39.5604395604395, 92.3076923076922, 76.9230769230769, 60.989010989011, 50.5494505494505, 39.5604395604395, 38.4615384615384, 39.0109890109889, 39.0109890109889, 43.9560439560439, 46.1538461538461, 60.4395604395604, 76.9230769230769, 90.6593406593406, 120.87912087912, 152.747252747252, 167.032967032967, 169.230769230769, 174.725274725274, 190.10989010989, 200.0, 208.791208791208, 208.241758241758, 201.648351648351, 190.10989010989, 186.813186813186, 171.978021978021, 149.450549450549, 126.373626373626, 35.1648351648351, 43.9560439560439, 76.9230769230769, 121.428571428571, 160.43956043956, 180.21978021978, 202.747252747252, 219.780219780219, 219.780219780219, 206.593406593406, 189.010989010989, 170.32967032967, 149.450549450549, 123.076923076923, 93.9560439560439, 67.5824175824175, 42.8571428571428, 31.3186813186813, 30.7692307692307, 31.3186813186813, 31.8681318681318, 33.5164835164835, 93.9560439560439, 67.5824175824175, 42.8571428571428, 31.3186813186813, 30.7692307692307, 31.3186813186813, 31.8681318681318, 33.5164835164835, 35.1648351648351, 43.9560439560439, 76.9230769230769, 121.428571428571, 160.43956043956, 180.21978021978, 202.747252747252, 219.780219780219, 219.780219780219, 206.593406593406, 189.010989010989, 170.32967032967, 149.450549450549, 123.076923076923, 25.2747252747252, 25.2747252747252, 25.8241758241758, 27.4725274725274, 29.6703296703296, 62.087912087912, 96.7032967032967, 124.725274725274, 152.747252747252, 179.120879120879, 197.252747252747, 215.384615384615, 225.274725274725, 229.670329670329, 219.780219780219, 201.098901098901, 172.527472527472, 139.560439560439, 97.8021978021978, 71.4285714285714, 45.054945054945, 30.7692307692307, 24.1758241758242, 97.8021978021978, 71.4285714285714, 45.054945054945, 30.7692307692307, 24.1758241758242, 25.2747252747252, 25.2747252747252, 25.8241758241758, 27.4725274725274, 29.6703296703296, 62.087912087912, 96.7032967032967, 124.725274725274, 152.747252747252, 179.120879120879, 197.252747252747, 215.384615384615, 225.274725274725, 229.670329670329, 219.780219780219, 201.098901098901, 172.527472527472, 139.560439560439, 21.9780219780219, 25.2747252747252, 47.8021978021978, 101.648351648351, 127.472527472527, 157.142857142857, 172.527472527472, 196.703296703296, 215.384615384615, 227.472527472527, 236.263736263736, 238.461538461538, 237.912087912087, 224.725274725274, 212.087912087912, 190.65934065934, 167.032967032967, 139.010989010989, 109.890109890109, 84.6153846153846, 56.5934065934066, 38.4615384615384, 24.1758241758242, 19.2307692307692, 19.2307692307692, 19.2307692307692, 20.8791208791209, 109.890109890109, 84.6153846153846, 56.5934065934066, 38.4615384615384, 24.1758241758242, 19.2307692307692, 19.2307692307692, 19.2307692307692, 20.8791208791209, 21.9780219780219, 25.2747252747252, 47.8021978021978, 101.648351648351, 127.472527472527, 157.142857142857, 172.527472527472, 196.703296703296, 215.384615384615, 227.472527472527, 236.263736263736, 238.461538461538, 237.912087912087, 224.725274725274, 212.087912087912, 190.65934065934, 167.032967032967, 139.010989010989, 9.34065934065932, 9.89010989010989, 7.69230769230767, 8.79120879120881, 9.89010989010989, 9.89010989010989, 10.4395604395604, 8.79120879120881, 13.1868131868131, 28.5714285714285, 68.1318681318681, 101.098901098901, 125.274725274725, 150.54945054945, 168.681318681318, 192.857142857142, 208.241758241758, 229.670329670329, 236.263736263736, 241.758241758241, 247.252747252747, 248.351648351648, 240.65934065934, 226.923076923076, 210.98901098901, 195.604395604395, 170.32967032967, 138.461538461538, 104.395604395604, 73.6263736263736, 45.6043956043955, 30.7692307692307, 14.8351648351648, 9.89010989010989, 104.395604395604, 73.6263736263736, 45.6043956043955, 30.7692307692307, 14.8351648351648, 9.89010989010989, 9.34065934065932, 9.89010989010989, 7.69230769230767, 8.79120879120881, 9.89010989010989, 9.89010989010989, 10.4395604395604, 8.79120879120881, 13.1868131868131, 28.5714285714285, 68.1318681318681, 101.098901098901, 125.274725274725, 150.54945054945, 168.681318681318, 192.857142857142, 208.241758241758, 229.670329670329, 236.263736263736, 241.758241758241, 247.252747252747, 248.351648351648, 240.65934065934, 226.923076923076, 210.98901098901, 195.604395604395, 170.32967032967, 138.461538461538, 0.0, 3.29670329670329, 6.04395604395603, 15.9340659340659, 39.0109890109889, 70.3296703296703, 160.43956043956, 205.494505494505, 236.263736263736, 249.450549450549, 253.296703296703, 253.296703296703, 247.802197802197, 237.362637362637, 231.868131868131, 224.175824175824, 210.98901098901, 194.505494505494, 171.428571428571, 148.351648351648, 84.065934065934, 59.8901098901099, 40.1098901098901, 23.076923076923, 10.4395604395604, 1.64835164835164, 0.0, 0.0, 0.0, 0.0 };
    static int16_T c2_inputVal[671] = { 214, 218, 218, 218, 218, 218, 218, 218, 218, 218, 218, 218, 218, 218, 218, 218, 218, 220, 220, 220, 220, 220, 220, 220, 220, 220, 220, 220, 220, 220, 220, 220, 220, 225, 225, 225, 225, 225, 225, 225, 225, 225, 225, 225, 225, 225, 225, 225, 225, 225, 225, 225, 225, 225, 225, 225, 225, 225, 225, 225, 225, 225, 225, 225, 225, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 235, 235, 235, 235, 235, 235, 235, 235, 235, 235, 235, 235, 235, 235, 235, 235, 235, 235, 235, 235, 235, 235, 235, 235, 235, 235, 235, 235, 235, 235, 235, 235, 235, 235, 235, 235, 235, 235, 235, 235, 235, 235, 235, 235, 235, 235, 235, 235, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 245, 245, 245, 245, 245, 245, 245, 245, 245, 245, 245, 245, 245, 245, 245, 245, 245, 245, 245, 245, 245, 245, 245, 245, 245, 245, 245, 245, 245, 245, 245, 245, 245, 245, 245, 245, 245, 245, 245, 245, 245, 245, 245, 245, 245, 245, 245, 245, 245, 245, 245, 245, 245, 245, 245, 245, 245, 245, 245, 245, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800 };
    static char_T c2_b[32] = { 'C', 'o', 'd', 'e', 'r', ':', 't', 'o', 'o', 'l', 'b', 'o', 'x', ':', 'Q', 'h', 'u', 'l', 'l', 'I', 'n', 't', 'e', 'r', 'n', 'a', 'l', 'E', 'r', 'r', 'o', 'r' };
    emlrtStack c2_b_st;
    emlrtStack c2_c_st;
    emlrtStack c2_st;
    const mxArray *c2_b_y = NULL;
    const mxArray *c2_c_y = NULL;
    const mxArray *c2_y = NULL;
    real_T c2_b_data[1342];
    real_T c2_obj_thePoints_data[1342];
    real_T c2_numDupsOfIdx_data[671];
    real_T c2_ycol[671];
    int32_T c2_idx[671];
    int32_T c2_ipos[671];
    int32_T c2_iwork[671];
    int32_T c2_iwork_data[671];
    int32_T c2_b_p;
    int32_T c2_i;
    int32_T c2_i2;
    int32_T c2_j;
    int32_T c2_k;
    int32_T c2_kEnd;
    int32_T c2_loop_ub;
    int32_T c2_nb;
    int32_T c2_pEnd;
    int32_T c2_q;
    int32_T c2_qEnd;
    int32_T exitg1;
    uint16_T c2_obj_idxMap[671];
    char_T c2_errMsg[4096];
    char_T c2_u_data[4096];
    boolean_T c2_p;
    boolean_T exitg2;
    c2_st.prev = c2_sp;
    c2_st.tls = c2_sp->tls;
    c2_b_st.prev = &c2_st;
    c2_b_st.tls = c2_st.tls;
    c2_c_st.prev = &c2_b_st;
    c2_c_st.tls = c2_b_st.tls;
    c2_st.site = &ptr_gvar_instance->c2_d_emlrtRSI;
    memset(&c2_idx[0], 0, 671U * sizeof(int32_T));
    for (c2_k = 0; c2_k <= 669; c2_k += 2) {
        if (c2_sortLE(S, ptr_gvar_instance, c2_c, c2_k + 1, c2_k + 2)) {
            c2_idx[c2_k] = c2_k + 1;
            c2_idx[c2_k + 1] = c2_k + 2;
        } else {
            c2_idx[c2_k] = c2_k + 2;
            c2_idx[c2_k + 1] = c2_k + 1;
        }
    }
    c2_idx[670] = 671;
    c2_i = 2;
    while (c2_i < 671) {
        c2_i2 = c2_i << 1;
        c2_j = 1;
        for (c2_pEnd = c2_i + 1; c2_pEnd < 672; c2_pEnd = c2_qEnd + c2_i) {
            c2_b_p = c2_j;
            c2_q = c2_pEnd;
            c2_qEnd = c2_j + c2_i2;
            if (c2_qEnd > 672) {
                c2_qEnd = 672;
            }
            c2_k = 0;
            c2_kEnd = c2_qEnd - c2_j;
            while (c2_k < c2_kEnd) {
                if (c2_sortLE(S, ptr_gvar_instance, c2_c, c2_idx[c2_b_p - 1], c2_idx[c2_q - 1])) {
                    c2_iwork[c2_k] = c2_idx[c2_b_p - 1];
                    c2_b_p++;
                    if (c2_b_p == c2_pEnd) {
                        while (c2_q < c2_qEnd) {
                            c2_k++;
                            c2_iwork[c2_k] = c2_idx[c2_q - 1];
                            c2_q++;
                        }
                    }
                } else {
                    c2_iwork[c2_k] = c2_idx[c2_q - 1];
                    c2_q++;
                    if (c2_q == c2_qEnd) {
                        while (c2_b_p < c2_pEnd) {
                            c2_k++;
                            c2_iwork[c2_k] = c2_idx[c2_b_p - 1];
                            c2_b_p++;
                        }
                    }
                }
                c2_k++;
            }
            for (c2_k = 0; c2_k < c2_kEnd; c2_k++) {
                c2_idx[(c2_j + c2_k) - 1] = c2_iwork[c2_k];
            }
            c2_j = c2_qEnd;
        }
        c2_i = c2_i2;
    }
    for (c2_pEnd = 0; c2_pEnd < 1342; c2_pEnd++) {
        c2_obj_thePoints_data[c2_pEnd] = c2_c[c2_pEnd];
    }
    for (c2_j = 0; c2_j < 2; c2_j++) {
        for (c2_i = 0; c2_i < 671; c2_i++) {
            c2_ycol[c2_i] = c2_obj_thePoints_data[(c2_idx[c2_i] + 671 * c2_j) - 1];
        }
        for (c2_i = 0; c2_i < 671; c2_i++) {
            c2_obj_thePoints_data[c2_i + 671 * c2_j] = c2_ycol[c2_i];
        }
    }
    for (c2_pEnd = 0; c2_pEnd < 671; c2_pEnd++) {
        c2_ycol[c2_pEnd] = (real_T)c2_idx[c2_pEnd];
    }
    for (c2_pEnd = 0; c2_pEnd < 1342; c2_pEnd++) {
        c2_b_data[c2_pEnd] = c2_obj_thePoints_data[c2_pEnd];
    }
    c2_nb = 0;
    c2_k = 1;
    while (c2_k <= 671) {
        c2_i2 = c2_k - 1;
        do {
            exitg1 = 0;
            c2_k++;
            if (c2_k > 671) {
                exitg1 = 1;
            } else {
                c2_p = false;
                c2_j = 0;
                exitg2 = false;
                while ((!exitg2) && (c2_j < 2)) {
                    if (c2_b_data[c2_i2 + 671 * c2_j] != c2_b_data[(c2_k + 671 * c2_j) - 1]) {
                        c2_p = true;
                        exitg2 = true;
                    } else {
                        c2_j++;
                    }
                }
                if (c2_p) {
                    exitg1 = 1;
                }
            }
        } while (exitg1 == 0);
        c2_nb++;
        for (c2_j = 0; c2_j < 2; c2_j++) {
            c2_b_data[(c2_nb + 671 * c2_j) - 1] = c2_b_data[c2_i2 + 671 * c2_j];
        }
        for (c2_j = c2_i2 + 1; c2_j < c2_k; c2_j++) {
            c2_ipos[(int32_T)c2_ycol[c2_j - 1] - 1] = c2_nb;
        }
        c2_ycol[c2_nb - 1] = c2_ycol[c2_i2];
    }
    if (c2_nb < 1) {
        c2_loop_ub = 0;
    } else {
        c2_loop_ub = c2_nb;
    }
    for (c2_pEnd = 0; c2_pEnd < 2; c2_pEnd++) {
        for (c2_i2 = 0; c2_i2 < c2_loop_ub; c2_i2++) {
            c2_b_data[c2_i2 + c2_loop_ub * c2_pEnd] = c2_b_data[c2_i2 + 671 * c2_pEnd];
        }
    }
    c2_pEnd = (int32_T)(uint16_T)c2_nb;
    for (c2_k = 0; c2_k < c2_pEnd; c2_k++) {
        c2_idx[c2_k] = (int32_T)c2_ycol[c2_k];
    }
    *c2_obj_delTri_numPts = (int32_T)(int16_T)c2_nb;
    c2_i2 = (int32_T)(int16_T)c2_nb;
    if (c2_i2 - 1 >= 0) {
        memset(&c2_iwork[0], 0, (uint32_T)c2_i2 * sizeof(int32_T));
    }
    if (c2_nb == 0) {
    } else {
        *c2_obj_delTri_numPts = (int32_T)(int16_T)c2_nb;
        c2_i2 = (int32_T)(int16_T)c2_nb;
        if (c2_i2 - 1 >= 0) {
            memset(&c2_iwork[0], 0, (uint32_T)c2_i2 * sizeof(int32_T));
        }
        c2_pEnd = c2_nb - 1;
        for (c2_k = 1; c2_k <= c2_pEnd; c2_k += 2) {
            if (c2_idx[c2_k - 1] <= c2_idx[c2_k]) {
                c2_iwork[c2_k - 1] = c2_k;
                c2_iwork[c2_k] = c2_k + 1;
            } else {
                c2_iwork[c2_k - 1] = c2_k + 1;
                c2_iwork[c2_k] = c2_k;
            }
        }
        if (((uint32_T)c2_nb & 1U) != 0U) {
            c2_iwork[c2_nb - 1] = c2_nb;
        }
        c2_i = 2;
        while (c2_i < c2_nb) {
            c2_i2 = c2_i << 1;
            c2_j = 1;
            for (c2_pEnd = c2_i + 1; c2_pEnd < c2_nb + 1; c2_pEnd = c2_qEnd + c2_i) {
                c2_b_p = c2_j;
                c2_q = c2_pEnd;
                c2_qEnd = c2_j + c2_i2;
                if (c2_qEnd > c2_nb + 1) {
                    c2_qEnd = c2_nb + 1;
                }
                c2_k = 0;
                c2_kEnd = c2_qEnd - c2_j;
                while (c2_k < c2_kEnd) {
                    if (c2_idx[c2_iwork[c2_b_p - 1] - 1] <= c2_idx[c2_iwork[c2_q - 1] - 1]) {
                        c2_iwork_data[c2_k] = c2_iwork[c2_b_p - 1];
                        c2_b_p++;
                        if (c2_b_p == c2_pEnd) {
                            while (c2_q < c2_qEnd) {
                                c2_k++;
                                c2_iwork_data[c2_k] = c2_iwork[c2_q - 1];
                                c2_q++;
                            }
                        }
                    } else {
                        c2_iwork_data[c2_k] = c2_iwork[c2_q - 1];
                        c2_q++;
                        if (c2_q == c2_qEnd) {
                            while (c2_b_p < c2_pEnd) {
                                c2_k++;
                                c2_iwork_data[c2_k] = c2_iwork[c2_b_p - 1];
                                c2_b_p++;
                            }
                        }
                    }
                    c2_k++;
                }
                for (c2_k = 0; c2_k < c2_kEnd; c2_k++) {
                    c2_iwork[(c2_j + c2_k) - 1] = c2_iwork_data[c2_k];
                }
                c2_j = c2_qEnd;
            }
            c2_i = c2_i2;
        }
    }
    c2_pEnd = (int32_T)(uint16_T)c2_nb;
    for (c2_k = 0; c2_k < c2_pEnd; c2_k++) {
        c2_idx[c2_iwork[c2_k] - 1] = c2_k + 1;
    }
    for (c2_pEnd = 0; c2_pEnd < 671; c2_pEnd++) {
        c2_ipos[c2_pEnd] = c2_idx[c2_ipos[c2_pEnd] - 1];
    }
    if (*c2_obj_delTri_numPts == 0) {
        c2_i2 = 0;
    } else if (*c2_obj_delTri_numPts > 2) {
        c2_i2 = *c2_obj_delTri_numPts;
    } else {
        c2_i2 = 2;
    }
    if (c2_i2 < 671) {
        *c2_obj_delTri_dupesExist = true;
    } else {
        *c2_obj_delTri_dupesExist = false;
    }
    for (c2_pEnd = 0; c2_pEnd < 671; c2_pEnd++) {
        c2_obj_idxMap[c2_pEnd] = (uint16_T)c2_ipos[c2_pEnd];
    }
    for (c2_pEnd = 0; c2_pEnd < *c2_obj_delTri_numPts; c2_pEnd++) {
        for (c2_i2 = 0; c2_i2 < 2; c2_i2++) {
            c2_obj_thePoints_data[c2_i2 + (c2_pEnd << 1)] = c2_b_data[(c2_iwork[c2_pEnd] + c2_loop_ub * c2_i2) - 1];
        }
    }
    c2_b_st.site = &ptr_gvar_instance->c2_e_emlrtRSI;
    c2_c_st.site = &ptr_gvar_instance->c2_g_emlrtRSI;
    c2_iobj_0->delTri = NULL;
    c2_iobj_0->convHull = NULL;
    c2_iobj_0->convHullTri = NULL;
    c2_c_st.site = &ptr_gvar_instance->c2_h_emlrtRSI;
    createQhullWrapperStructs(&c2_iobj_0->delTri, &c2_iobj_0->convHull, &c2_iobj_0->convHullTri);
    c2_iobj_0->matlabCodegenIsDeleted = false;
    c2_b_st.site = &ptr_gvar_instance->c2_f_emlrtRSI;
    c2_i2 = computeDelaunayTri(&c2_iobj_0->delTri, &c2_obj_thePoints_data[0], 2, *c2_obj_delTri_numPts, "Qt Qbb Qc");
    if (c2_i2 > 0) {
        c2_i2 = computeDelaunayTri(&c2_iobj_0->delTri, &c2_obj_thePoints_data[0], 2, *c2_obj_delTri_numPts, "Qt Qbb Qc Qz");
        if (c2_i2 > 0) {
            c2_c_st.site = &ptr_gvar_instance->c2_i_emlrtRSI;
            c2_i2 = getQhullErrorStr(&c2_errMsg[0]);
            if (c2_i2 < 1) {
                c2_i2 = 0;
            }
            for (c2_pEnd = 0; c2_pEnd < c2_i2; c2_pEnd++) {
                c2_u_data[c2_pEnd] = c2_errMsg[c2_pEnd];
            }
            c2_c_y = NULL;
            sf_mex_assign(&c2_c_y, sf_mex_create("y", &c2_u_data, 10, 0U, 1, 0U, 2, 1, c2_i2), false);
            sf_mex_call(&c2_c_st, NULL, "disp", 0U, 1U, 14, c2_c_y);
            c2_y = NULL;
            sf_mex_assign(&c2_y, sf_mex_create("y", c2_b, 10, 0U, 1, 0U, 2, 1, 32), false);
            c2_b_y = NULL;
            sf_mex_assign(&c2_b_y, sf_mex_create("y", c2_b, 10, 0U, 1, 0U, 2, 1, 32), false);
            sf_mex_call(&c2_b_st, &ptr_gvar_instance->c2_b_emlrtMCI, "error", 0U, 2U, 14, c2_y, 14, sf_mex_call(&c2_b_st, NULL, "getString", 1U, 1U, 14, sf_mex_call(&c2_b_st, NULL, "message", 1U, 1U, 14, c2_b_y)));
        }
    }
    c2_obj_delTri_thePoints_size[0] = 2;
    c2_obj_delTri_thePoints_size[1] = *c2_obj_delTri_numPts;
    c2_loop_ub = *c2_obj_delTri_numPts << 1;
    for (c2_pEnd = 0; c2_pEnd < c2_loop_ub; c2_pEnd++) {
        c2_obj_delTri_thePoints_data[c2_pEnd] = c2_obj_thePoints_data[c2_pEnd];
    }
    *c2_obj_delTri_qhWrapper = c2_iobj_0;
    for (c2_pEnd = 0; c2_pEnd < 671; c2_pEnd++) {
        c2_obj_delTri_idxMap[c2_pEnd] = (real_T)c2_obj_idxMap[c2_pEnd];
    }
    c2_st.site = &ptr_gvar_instance->c2_c_emlrtRSI;
    if (*c2_obj_delTri_dupesExist) {
        c2_b_st.site = &ptr_gvar_instance->c2_k_emlrtRSI;
        c2_warning(S, ptr_gvar_instance, &c2_b_st);
        c2_i2 = *c2_obj_delTri_numPts;
        if (*c2_obj_delTri_numPts - 1 >= 0) {
            memset(&c2_ycol[0], 0, (uint32_T)*c2_obj_delTri_numPts * sizeof(real_T));
        }
        if (*c2_obj_delTri_numPts - 1 >= 0) {
            memset(&c2_numDupsOfIdx_data[0], 0, (uint32_T)*c2_obj_delTri_numPts * sizeof(real_T));
        }
        for (c2_i = 0; c2_i < 671; c2_i++) {
            c2_numDupsOfIdx_data[(int32_T)c2_obj_idxMap[c2_i] - 1]++;
            c2_ycol[(int32_T)c2_obj_idxMap[c2_i] - 1] += (real_T)c2_inputVal[c2_i];
        }
        for (c2_i = 0; c2_i < *c2_obj_delTri_numPts; c2_i++) {
            if (muDoubleScalarIsInf(c2_ycol[c2_i])) {
                c2_ycol[c2_i] = rtNaN;
            } else if (!muDoubleScalarIsNaN(c2_ycol[c2_i])) {
                c2_ycol[c2_i] /= c2_numDupsOfIdx_data[c2_i];
            }
        }
    } else {
        c2_i2 = 671;
        for (c2_pEnd = 0; c2_pEnd < 671; c2_pEnd++) {
            c2_ycol[c2_pEnd] = (real_T)c2_inputVal[c2_pEnd];
        }
    }
    c2_obj_sampleVal_size[0] = c2_i2;
    for (c2_pEnd = 0; c2_pEnd < c2_i2; c2_pEnd++) {
        c2_obj_sampleVal_data[c2_pEnd] = c2_ycol[c2_pEnd];
    }
    *c2_obj_validInterpolant = true;
}

void c2_warning(SimStruct *S, gvar_instance *ptr_gvar_instance, const emlrtStack *c2_sp)
{
    static char_T c2_msgID[44] = { 'M', 'A', 'T', 'L', 'A', 'B', ':', 'm', 'a', 't', 'h', 'c', 'g', 'e', 'o', '_', 'c', 'a', 't', 'a', 'l', 'o', 'g', ':', 'D', 'u', 'p', 'P', 't', 's', 'A', 'v', 'V', 'a', 'l', 'u', 'e', 's', 'W', 'a', 'r', 'n', 'I', 'd' };
    static char_T c2_b[7] = { 'w', 'a', 'r', 'n', 'i', 'n', 'g' };
    static char_T c2_c[7] = { 'm', 'e', 's', 's', 'a', 'g', 'e' };
    emlrtStack c2_st;
    const mxArray *c2_b_y = NULL;
    const mxArray *c2_c_y = NULL;
    const mxArray *c2_y = NULL;
    c2_st.prev = c2_sp;
    c2_st.tls = c2_sp->tls;
    c2_y = NULL;
    sf_mex_assign(&c2_y, sf_mex_create("y", c2_b, 10, 0U, 1, 0U, 2, 1, 7), false);
    c2_b_y = NULL;
    sf_mex_assign(&c2_b_y, sf_mex_create("y", c2_c, 10, 0U, 1, 0U, 2, 1, 7), false);
    c2_c_y = NULL;
    sf_mex_assign(&c2_c_y, sf_mex_create("y", c2_msgID, 10, 0U, 1, 0U, 2, 1, 44), false);
    c2_st.site = &ptr_gvar_instance->c2_j_emlrtRSI;
    c2_b_feval(S, ptr_gvar_instance, &c2_st, c2_y, c2_feval(S, ptr_gvar_instance, &c2_st, c2_b_y, c2_c_y));
}

void c2_b_warning(SimStruct *S, gvar_instance *ptr_gvar_instance, const emlrtStack *c2_sp)
{
    static char_T c2_msgID[21] = { 'M', 'A', 'T', 'L', 'A', 'B', ':', 's', 'i', 'n', 'g', 'u', 'l', 'a', 'r', 'M', 'a', 't', 'r', 'i', 'x' };
    static char_T c2_b[7] = { 'w', 'a', 'r', 'n', 'i', 'n', 'g' };
    static char_T c2_c[7] = { 'm', 'e', 's', 's', 'a', 'g', 'e' };
    emlrtStack c2_st;
    const mxArray *c2_b_y = NULL;
    const mxArray *c2_c_y = NULL;
    const mxArray *c2_y = NULL;
    c2_st.prev = c2_sp;
    c2_st.tls = c2_sp->tls;
    c2_y = NULL;
    sf_mex_assign(&c2_y, sf_mex_create("y", c2_b, 10, 0U, 1, 0U, 2, 1, 7), false);
    c2_b_y = NULL;
    sf_mex_assign(&c2_b_y, sf_mex_create("y", c2_c, 10, 0U, 1, 0U, 2, 1, 7), false);
    c2_c_y = NULL;
    sf_mex_assign(&c2_c_y, sf_mex_create("y", c2_msgID, 10, 0U, 1, 0U, 2, 1, 21), false);
    c2_st.site = &ptr_gvar_instance->c2_j_emlrtRSI;
    c2_d_feval(S, ptr_gvar_instance, &c2_st, c2_y, c2_c_feval(S, ptr_gvar_instance, &c2_st, c2_b_y, c2_c_y));
}

real_T c2_emlrt_marshallIn(SimStruct *S, gvar_instance *ptr_gvar_instance, const mxArray *c2_nullptr, const char_T *c2_identifier)
{
    emlrtMsgIdentifier c2_thisId;
    real_T c2_y;
    c2_thisId.fIdentifier = (const char_T *)c2_identifier;
    c2_thisId.fParent = NULL;
    c2_thisId.bParentIsCell = false;
    c2_y = c2_b_emlrt_marshallIn(S, ptr_gvar_instance, sf_mex_dup(c2_nullptr), &c2_thisId);
    sf_mex_destroy(&c2_nullptr);
    return c2_y;
}

real_T c2_b_emlrt_marshallIn(SimStruct *S, gvar_instance *ptr_gvar_instance, const mxArray *c2_u, const emlrtMsgIdentifier *c2_parentId)
{
    real_T c2_b;
    real_T c2_y;
    (void)S;
    (void)ptr_gvar_instance;
    sf_mex_import(c2_parentId, sf_mex_dup(c2_u), &c2_b, 1, 0, 0U, 0, 0U, 0);
    c2_y = c2_b;
    sf_mex_destroy(&c2_u);
    return c2_y;
}

real_T c2_c_emlrt_marshallIn(SimStruct *S, gvar_instance *ptr_gvar_instance, const mxArray *c2_nullptr, const char_T *c2_identifier, boolean_T *c2_svPtr)
{
    emlrtMsgIdentifier c2_thisId;
    real_T c2_y;
    c2_thisId.fIdentifier = (const char_T *)c2_identifier;
    c2_thisId.fParent = NULL;
    c2_thisId.bParentIsCell = false;
    c2_y = c2_d_emlrt_marshallIn(S, ptr_gvar_instance, sf_mex_dup(c2_nullptr), &c2_thisId, c2_svPtr);
    sf_mex_destroy(&c2_nullptr);
    return c2_y;
}

real_T c2_d_emlrt_marshallIn(SimStruct *S, gvar_instance *ptr_gvar_instance, const mxArray *c2_u, const emlrtMsgIdentifier *c2_parentId, boolean_T *c2_svPtr)
{
    real_T c2_b;
    real_T c2_y;
    (void)S;
    (void)ptr_gvar_instance;
    if (mxIsEmpty(c2_u)) {
        *c2_svPtr = false;
    } else {
        *c2_svPtr = true;
        sf_mex_import(c2_parentId, sf_mex_dup(c2_u), &c2_b, 1, 0, 0U, 0, 0U, 0);
        c2_y = c2_b;
    }
    sf_mex_destroy(&c2_u);
    return c2_y;
}

const mxArray *c2_feval(SimStruct *S, gvar_instance *ptr_gvar_instance, const emlrtStack *c2_sp, const mxArray *c2_input0, const mxArray *c2_input1)
{
    const mxArray *c2_ = NULL;
    (void)S;
    (void)ptr_gvar_instance;
    c2_ = NULL;
    sf_mex_assign(&c2_, sf_mex_call(c2_sp, NULL, "feval", 1U, 2U, 14, sf_mex_dup(c2_input0), 14, sf_mex_dup(c2_input1)), false);
    sf_mex_destroy(&c2_input0);
    sf_mex_destroy(&c2_input1);
    return c2_;
}

void c2_b_feval(SimStruct *S, gvar_instance *ptr_gvar_instance, const emlrtStack *c2_sp, const mxArray *c2_input0, const mxArray *c2_input1)
{
    (void)S;
    (void)ptr_gvar_instance;
    sf_mex_call(c2_sp, NULL, "feval", 0U, 2U, 14, sf_mex_dup(c2_input0), 14, sf_mex_dup(c2_input1));
    sf_mex_destroy(&c2_input0);
    sf_mex_destroy(&c2_input1);
}

const mxArray *c2_c_feval(SimStruct *S, gvar_instance *ptr_gvar_instance, const emlrtStack *c2_sp, const mxArray *c2_input0, const mxArray *c2_input1)
{
    const mxArray *c2_ = NULL;
    (void)S;
    (void)ptr_gvar_instance;
    c2_ = NULL;
    sf_mex_assign(&c2_, sf_mex_call(c2_sp, NULL, "feval", 1U, 2U, 14, sf_mex_dup(c2_input0), 14, sf_mex_dup(c2_input1)), false);
    sf_mex_destroy(&c2_input0);
    sf_mex_destroy(&c2_input1);
    return c2_;
}

void c2_d_feval(SimStruct *S, gvar_instance *ptr_gvar_instance, const emlrtStack *c2_sp, const mxArray *c2_input0, const mxArray *c2_input1)
{
    (void)S;
    (void)ptr_gvar_instance;
    sf_mex_call(c2_sp, NULL, "feval", 0U, 2U, 14, sf_mex_dup(c2_input0), 14, sf_mex_dup(c2_input1));
    sf_mex_destroy(&c2_input0);
    sf_mex_destroy(&c2_input1);
}

void c2_array_scatteredInterpolant_Cons(SimStruct *S, gvar_instance *ptr_gvar_instance, c2_scatteredInterpolant *c2_pStruct)
{
    c2_array_real_T_671_Constructor(S, ptr_gvar_instance, &c2_pStruct->sampleVal);
    c2_array_coder_internal_delaunayTr(S, ptr_gvar_instance, &c2_pStruct->delTri);
}

void c2_array_real_T_671_Constructor(SimStruct *S, gvar_instance *ptr_gvar_instance, c2_coder_array_real_T_671 *c2_pArray)
{
    (void)S;
    (void)ptr_gvar_instance;
    c2_pArray->size[0] = 0;
}

void c2_array_coder_internal_delaunayTr(SimStruct *S, gvar_instance *ptr_gvar_instance, c2_coder_internal_delaunayTriangulation *c2_pStruct)
{
    c2_array_real_T_2x671_Constructor(S, ptr_gvar_instance, &c2_pStruct->thePoints);
}

void c2_array_real_T_2x671_Constructor(SimStruct *S, gvar_instance *ptr_gvar_instance, c2_coder_array_real_T_2x671 *c2_pArray)
{
    (void)S;
    (void)ptr_gvar_instance;
    c2_pArray->size[0] = 0;
    c2_pArray->size[1] = 0;
}

void init_dsm_address_info(SimStruct *S, gvar_instance *ptr_gvar_instance)
{
    (void)S;
    (void)ptr_gvar_instance;
}

void init_simulink_io_address(SimStruct *S, gvar_instance *ptr_gvar_instance)
{
    ptr_gvar_instance->c2_fEmlrtCtx = (void *)sfrtGetEmlrtCtx(S);
    ptr_gvar_instance->c2_rpm = (real_T *)ssGetInputPortSignal_wrapper(S, 0);
    ptr_gvar_instance->c2_bsfc = (real_T *)ssGetOutputPortSignal_wrapper(S, 1);
    ptr_gvar_instance->c2_torque = (real_T *)ssGetInputPortSignal_wrapper(S, 1);
}

void JIT_release_mem_fcn(gvar_instance *ptr_gvar_instance)
{
    free(ptr_gvar_instance);
}

gvar_instance *JIT_init_mem_fcn(void)
{
    gvar_instance *ptr_gvar_instance;
    ptr_gvar_instance = (gvar_instance *)calloc((size_t)1U, sizeof(gvar_instance));
    ptr_gvar_instance->c2_ab_emlrtRSI.lineNo = 42;
    ptr_gvar_instance->c2_ab_emlrtRSI.fcnName = "mrdivide_helper";
    ptr_gvar_instance->c2_ab_emlrtRSI.pathName = "C:\\Program Files\\MATLAB\\R2025a\\toolbox\\eml\\eml\\+coder\\+internal\\mrdivide_helper.m";
    ptr_gvar_instance->c2_b_emlrtMCI.lineNo = 49;
    ptr_gvar_instance->c2_b_emlrtMCI.colNo = 21;
    ptr_gvar_instance->c2_b_emlrtMCI.fName = "scatteredInterpAPI";
    ptr_gvar_instance->c2_b_emlrtMCI.pName = "C:\\Program Files\\MATLAB\\R2025a\\toolbox\\eml\\eml\\+coder\\+internal\\scatteredInterpAPI.m";
    ptr_gvar_instance->c2_b_emlrtRSI.lineNo = 11;
    ptr_gvar_instance->c2_b_emlrtRSI.fcnName = "MATLAB Function";
    ptr_gvar_instance->c2_b_emlrtRSI.pathName = "#BSFC_simulink:1";
    ptr_gvar_instance->c2_b_emlrtRTEI.lineNo = 304;
    ptr_gvar_instance->c2_b_emlrtRTEI.colNo = 2;
    ptr_gvar_instance->c2_b_emlrtRTEI.fName = "unique";
    ptr_gvar_instance->c2_b_emlrtRTEI.pName = "C:\\Program Files\\MATLAB\\R2025a\\toolbox\\eml\\lib\\matlab\\ops\\unique.m";
    ptr_gvar_instance->c2_bb_emlrtRSI.lineNo = 61;
    ptr_gvar_instance->c2_bb_emlrtRSI.fcnName = "lusolve";
    ptr_gvar_instance->c2_bb_emlrtRSI.pathName = "C:\\Program Files\\MATLAB\\R2025a\\toolbox\\eml\\eml\\+coder\\+internal\\lusolve.m";
    ptr_gvar_instance->c2_c_emlrtMCI.lineNo = 178;
    ptr_gvar_instance->c2_c_emlrtMCI.colNo = 47;
    ptr_gvar_instance->c2_c_emlrtMCI.fName = "scatteredInterpAPI";
    ptr_gvar_instance->c2_c_emlrtMCI.pName = "C:\\Program Files\\MATLAB\\R2025a\\toolbox\\eml\\eml\\+coder\\+internal\\scatteredInterpAPI.m";
    ptr_gvar_instance->c2_c_emlrtRSI.lineNo = 152;
    ptr_gvar_instance->c2_c_emlrtRSI.fcnName = "scatteredInterpolant";
    ptr_gvar_instance->c2_c_emlrtRSI.pathName = "C:\\Program Files\\MATLAB\\R2025a\\toolbox\\shared\\coder\\coder\\+coder\\+internal\\@scatteredInterpolant\\scatteredInterpolant"
".m";
    ptr_gvar_instance->c2_c_emlrtRTEI.lineNo = 338;
    ptr_gvar_instance->c2_c_emlrtRTEI.colNo = 1;
    ptr_gvar_instance->c2_c_emlrtRTEI.fName = "unique";
    ptr_gvar_instance->c2_c_emlrtRTEI.pName = "C:\\Program Files\\MATLAB\\R2025a\\toolbox\\eml\\lib\\matlab\\ops\\unique.m";
    ptr_gvar_instance->c2_cb_emlrtRSI.lineNo = 293;
    ptr_gvar_instance->c2_cb_emlrtRSI.fcnName = "lusolve";
    ptr_gvar_instance->c2_cb_emlrtRSI.pathName = "C:\\Program Files\\MATLAB\\R2025a\\toolbox\\eml\\eml\\+coder\\+internal\\lusolve.m";
    ptr_gvar_instance->c2_d_emlrtMCI.lineNo = 181;
    ptr_gvar_instance->c2_d_emlrtMCI.colNo = 47;
    ptr_gvar_instance->c2_d_emlrtMCI.fName = "scatteredInterpAPI";
    ptr_gvar_instance->c2_d_emlrtMCI.pName = "C:\\Program Files\\MATLAB\\R2025a\\toolbox\\eml\\eml\\+coder\\+internal\\scatteredInterpAPI.m";
    ptr_gvar_instance->c2_d_emlrtRSI.lineNo = 148;
    ptr_gvar_instance->c2_d_emlrtRSI.fcnName = "scatteredInterpolant";
    ptr_gvar_instance->c2_d_emlrtRSI.pathName = "C:\\Program Files\\MATLAB\\R2025a\\toolbox\\shared\\coder\\coder\\+coder\\+internal\\@scatteredInterpolant\\scatteredInterpolant"
".m";
    ptr_gvar_instance->c2_d_emlrtRTEI.lineNo = 339;
    ptr_gvar_instance->c2_d_emlrtRTEI.colNo = 22;
    ptr_gvar_instance->c2_d_emlrtRTEI.fName = "unique";
    ptr_gvar_instance->c2_d_emlrtRTEI.pName = "C:\\Program Files\\MATLAB\\R2025a\\toolbox\\eml\\lib\\matlab\\ops\\unique.m";
    ptr_gvar_instance->c2_db_emlrtRSI.lineNo = 90;
    ptr_gvar_instance->c2_db_emlrtRSI.fcnName = "lusolve";
    ptr_gvar_instance->c2_db_emlrtRSI.pathName = "C:\\Program Files\\MATLAB\\R2025a\\toolbox\\eml\\eml\\+coder\\+internal\\lusolve.m";
    ptr_gvar_instance->c2_e_emlrtMCI.lineNo = 184;
    ptr_gvar_instance->c2_e_emlrtMCI.colNo = 47;
    ptr_gvar_instance->c2_e_emlrtMCI.fName = "scatteredInterpAPI";
    ptr_gvar_instance->c2_e_emlrtMCI.pName = "C:\\Program Files\\MATLAB\\R2025a\\toolbox\\eml\\eml\\+coder\\+internal\\scatteredInterpAPI.m";
    ptr_gvar_instance->c2_e_emlrtRSI.lineNo = 62;
    ptr_gvar_instance->c2_e_emlrtRSI.fcnName = "delaunayTriangulation";
    ptr_gvar_instance->c2_e_emlrtRSI.pathName = "C:\\Program Files\\MATLAB\\R2025a\\toolbox\\shared\\coder\\coder\\+coder\\+internal\\@delaunayTriangulation\\delaunayTriangulati"
"on.m";
    ptr_gvar_instance->c2_e_emlrtRTEI.lineNo = 345;
    ptr_gvar_instance->c2_e_emlrtRTEI.colNo = 5;
    ptr_gvar_instance->c2_e_emlrtRTEI.fName = "unique";
    ptr_gvar_instance->c2_e_emlrtRTEI.pName = "C:\\Program Files\\MATLAB\\R2025a\\toolbox\\eml\\lib\\matlab\\ops\\unique.m";
    ptr_gvar_instance->c2_eb_emlrtRSI.lineNo = 63;
    ptr_gvar_instance->c2_eb_emlrtRSI.fcnName = "function_handle";
    ptr_gvar_instance->c2_eb_emlrtRSI.pathName = "C:\\Program Files\\MATLAB\\R2025a\\toolbox\\eml\\eml\\+coder\\+internal\\function_handle.m";
    ptr_gvar_instance->c2_emlrtMCI.lineNo = 87;
    ptr_gvar_instance->c2_emlrtMCI.colNo = 33;
    ptr_gvar_instance->c2_emlrtMCI.fName = "eml_int_forloop_overflow_check";
    ptr_gvar_instance->c2_emlrtMCI.pName = "C:\\Program Files\\MATLAB\\R2025a\\toolbox\\eml\\lib\\matlab\\eml\\eml_int_forloop_overflow_check.m";
    ptr_gvar_instance->c2_emlrtRSI.lineNo = 27;
    ptr_gvar_instance->c2_emlrtRSI.fcnName = "MATLAB Function";
    ptr_gvar_instance->c2_emlrtRSI.pathName = "#BSFC_simulink:1";
    ptr_gvar_instance->c2_emlrtRTEI.lineNo = 11;
    ptr_gvar_instance->c2_emlrtRTEI.colNo = 13;
    ptr_gvar_instance->c2_emlrtRTEI.fName = "MATLAB Function";
    ptr_gvar_instance->c2_emlrtRTEI.pName = "#BSFC_simulink:1";
    ptr_gvar_instance->c2_f_emlrtRSI.lineNo = 63;
    ptr_gvar_instance->c2_f_emlrtRSI.fcnName = "delaunayTriangulation";
    ptr_gvar_instance->c2_f_emlrtRSI.pathName = "C:\\Program Files\\MATLAB\\R2025a\\toolbox\\shared\\coder\\coder\\+coder\\+internal\\@delaunayTriangulation\\delaunayTriangulati"
"on.m";
    ptr_gvar_instance->c2_f_emlrtRTEI.lineNo = 52;
    ptr_gvar_instance->c2_f_emlrtRTEI.colNo = 9;
    ptr_gvar_instance->c2_f_emlrtRTEI.fName = "mergesort";
    ptr_gvar_instance->c2_f_emlrtRTEI.pName = "C:\\Program Files\\MATLAB\\R2025a\\toolbox\\eml\\eml\\+coder\\+internal\\mergesort.m";
    ptr_gvar_instance->c2_fb_emlrtRSI.lineNo = 15;
    ptr_gvar_instance->c2_fb_emlrtRSI.fcnName = "nearestExtrapolationKernel";
    ptr_gvar_instance->c2_fb_emlrtRSI.pathName = "C:\\Program Files\\MATLAB\\R2025a\\toolbox\\shared\\coder\\coder\\+coder\\+internal\\@scatteredInterpolant\\nearestExtrapolation"
"Kernel.m";
    ptr_gvar_instance->c2_g_emlrtRSI.lineNo = 16;
    ptr_gvar_instance->c2_g_emlrtRSI.fcnName = "qhullStructsWrapper";
    ptr_gvar_instance->c2_g_emlrtRSI.pathName = "C:\\Program Files\\MATLAB\\R2025a\\toolbox\\shared\\coder\\coder\\+coder\\+internal\\qhullStructsWrapper.m";
    ptr_gvar_instance->c2_g_emlrtRTEI.lineNo = 347;
    ptr_gvar_instance->c2_g_emlrtRTEI.colNo = 27;
    ptr_gvar_instance->c2_g_emlrtRTEI.fName = "unique";
    ptr_gvar_instance->c2_g_emlrtRTEI.pName = "C:\\Program Files\\MATLAB\\R2025a\\toolbox\\eml\\lib\\matlab\\ops\\unique.m";
    ptr_gvar_instance->c2_gb_emlrtRSI.lineNo = 81;
    ptr_gvar_instance->c2_gb_emlrtRSI.fcnName = "delaunayTriangulation";
    ptr_gvar_instance->c2_gb_emlrtRSI.pathName = "C:\\Program Files\\MATLAB\\R2025a\\toolbox\\shared\\coder\\coder\\+coder\\+internal\\@delaunayTriangulation\\delaunayTriangulati"
"on.m";
    ptr_gvar_instance->c2_h_emlrtRSI.lineNo = 22;
    ptr_gvar_instance->c2_h_emlrtRSI.fcnName = "qhullStructsWrapper";
    ptr_gvar_instance->c2_h_emlrtRSI.pathName = "C:\\Program Files\\MATLAB\\R2025a\\toolbox\\shared\\coder\\coder\\+coder\\+internal\\qhullStructsWrapper.m";
    ptr_gvar_instance->c2_h_emlrtRTEI.lineNo = 47;
    ptr_gvar_instance->c2_h_emlrtRTEI.colNo = 21;
    ptr_gvar_instance->c2_h_emlrtRTEI.fName = "delaunayTriangulation";
    ptr_gvar_instance->c2_h_emlrtRTEI.pName = "C:\\Program Files\\MATLAB\\R2025a\\toolbox\\shared\\coder\\coder\\+coder\\+internal\\@delaunayTriangulation\\delaunayTriangulati"
"on.m";
    ptr_gvar_instance->c2_hb_emlrtRSI.lineNo = 24;
    ptr_gvar_instance->c2_hb_emlrtRSI.fcnName = "matlabCodegenHandle";
    ptr_gvar_instance->c2_hb_emlrtRSI.pathName = "C:\\Program Files\\MATLAB\\R2025a\\toolbox\\eml\\eml\\+coder\\+internal\\matlabCodegenHandle.m";
    ptr_gvar_instance->c2_i_emlrtRSI.lineNo = 48;
    ptr_gvar_instance->c2_i_emlrtRSI.fcnName = "scatteredInterpAPI";
    ptr_gvar_instance->c2_i_emlrtRSI.pathName = "C:\\Program Files\\MATLAB\\R2025a\\toolbox\\eml\\eml\\+coder\\+internal\\scatteredInterpAPI.m";
    ptr_gvar_instance->c2_i_emlrtRTEI.lineNo = 194;
    ptr_gvar_instance->c2_i_emlrtRTEI.colNo = 18;
    ptr_gvar_instance->c2_i_emlrtRTEI.fName = "scatteredInterpAPI";
    ptr_gvar_instance->c2_i_emlrtRTEI.pName = "C:\\Program Files\\MATLAB\\R2025a\\toolbox\\eml\\eml\\+coder\\+internal\\scatteredInterpAPI.m";
    ptr_gvar_instance->c2_ib_emlrtRSI.lineNo = 26;
    ptr_gvar_instance->c2_ib_emlrtRSI.fcnName = "qhullStructsWrapper";
    ptr_gvar_instance->c2_ib_emlrtRSI.pathName = "C:\\Program Files\\MATLAB\\R2025a\\toolbox\\shared\\coder\\coder\\+coder\\+internal\\qhullStructsWrapper.m";
    ptr_gvar_instance->c2_j_emlrtRSI.lineNo = 14;
    ptr_gvar_instance->c2_j_emlrtRSI.fcnName = "warning";
    ptr_gvar_instance->c2_j_emlrtRSI.pathName = "C:\\Program Files\\MATLAB\\R2025a\\toolbox\\shared\\coder\\coder\\lib\\+coder\\+internal\\warning.m";
    ptr_gvar_instance->c2_j_emlrtRTEI.lineNo = 148;
    ptr_gvar_instance->c2_j_emlrtRTEI.colNo = 17;
    ptr_gvar_instance->c2_j_emlrtRTEI.fName = "scatteredInterpolant";
    ptr_gvar_instance->c2_j_emlrtRTEI.pName = "C:\\Program Files\\MATLAB\\R2025a\\toolbox\\shared\\coder\\coder\\+coder\\+internal\\@scatteredInterpolant\\scatteredInterpolant"
".m";
    ptr_gvar_instance->c2_jb_emlrtRSI.lineNo = 1;
    ptr_gvar_instance->c2_jb_emlrtRSI.fcnName = "MATLAB Function";
    ptr_gvar_instance->c2_jb_emlrtRSI.pathName = "#BSFC_simulink:1";
    ptr_gvar_instance->c2_k_emlrtRSI.lineNo = 9;
    ptr_gvar_instance->c2_k_emlrtRSI.fcnName = "averageValuesForDuplicates";
    ptr_gvar_instance->c2_k_emlrtRSI.pathName = "C:\\Program Files\\MATLAB\\R2025a\\toolbox\\shared\\coder\\coder\\+coder\\+internal\\@scatteredInterpolant\\averageValuesForDupl"
"icates.m";
    ptr_gvar_instance->c2_k_emlrtRTEI.lineNo = 41;
    ptr_gvar_instance->c2_k_emlrtRTEI.colNo = 5;
    ptr_gvar_instance->c2_k_emlrtRTEI.fName = "averageValuesForDuplicates";
    ptr_gvar_instance->c2_k_emlrtRTEI.pName = "C:\\Program Files\\MATLAB\\R2025a\\toolbox\\shared\\coder\\coder\\+coder\\+internal\\@scatteredInterpolant\\averageValuesForDupl"
"icates.m";
    ptr_gvar_instance->c2_l_emlrtRSI.lineNo = 53;
    ptr_gvar_instance->c2_l_emlrtRSI.fcnName = "parenReference";
    ptr_gvar_instance->c2_l_emlrtRSI.pathName = "C:\\Program Files\\MATLAB\\R2025a\\toolbox\\shared\\coder\\coder\\+coder\\+internal\\@scatteredInterpolant\\parenReference.m";
    ptr_gvar_instance->c2_l_emlrtRTEI.lineNo = 20;
    ptr_gvar_instance->c2_l_emlrtRTEI.colNo = 5;
    ptr_gvar_instance->c2_l_emlrtRTEI.fName = "averageValuesForDuplicates";
    ptr_gvar_instance->c2_l_emlrtRTEI.pName = "C:\\Program Files\\MATLAB\\R2025a\\toolbox\\shared\\coder\\coder\\+coder\\+internal\\@scatteredInterpolant\\averageValuesForDupl"
"icates.m";
    ptr_gvar_instance->c2_m_emlrtRSI.lineNo = 16;
    ptr_gvar_instance->c2_m_emlrtRSI.fcnName = "anonymous_function";
    ptr_gvar_instance->c2_m_emlrtRSI.pathName = "C:\\Program Files\\MATLAB\\R2025a\\toolbox\\eml\\eml\\+coder\\+internal\\anonymous_function.m";
    ptr_gvar_instance->c2_m_emlrtRTEI.lineNo = 21;
    ptr_gvar_instance->c2_m_emlrtRTEI.colNo = 5;
    ptr_gvar_instance->c2_m_emlrtRTEI.fName = "averageValuesForDuplicates";
    ptr_gvar_instance->c2_m_emlrtRTEI.pName = "C:\\Program Files\\MATLAB\\R2025a\\toolbox\\shared\\coder\\coder\\+coder\\+internal\\@scatteredInterpolant\\averageValuesForDupl"
"icates.m";
    ptr_gvar_instance->c2_n_emlrtRSI.lineNo = 11;
    ptr_gvar_instance->c2_n_emlrtRSI.fcnName = "function_handle";
    ptr_gvar_instance->c2_n_emlrtRSI.pathName = "C:\\Program Files\\MATLAB\\R2025a\\toolbox\\eml\\eml\\+coder\\+internal\\function_handle.m";
    ptr_gvar_instance->c2_n_emlrtRTEI.lineNo = 152;
    ptr_gvar_instance->c2_n_emlrtRTEI.colNo = 17;
    ptr_gvar_instance->c2_n_emlrtRTEI.fName = "scatteredInterpolant";
    ptr_gvar_instance->c2_n_emlrtRTEI.pName = "C:\\Program Files\\MATLAB\\R2025a\\toolbox\\shared\\coder\\coder\\+coder\\+internal\\@scatteredInterpolant\\scatteredInterpolant"
".m";
    ptr_gvar_instance->c2_o_emlrtRSI.lineNo = 61;
    ptr_gvar_instance->c2_o_emlrtRSI.fcnName = "evalScatteredData";
    ptr_gvar_instance->c2_o_emlrtRSI.pathName = "C:\\Program Files\\MATLAB\\R2025a\\toolbox\\shared\\coder\\coder\\+coder\\+internal\\@scatteredInterpolant\\evalScatteredData.m";
    ptr_gvar_instance->c2_o_emlrtRTEI.lineNo = 3;
    ptr_gvar_instance->c2_o_emlrtRTEI.colNo = 16;
    ptr_gvar_instance->c2_o_emlrtRTEI.fName = "MATLAB Function";
    ptr_gvar_instance->c2_o_emlrtRTEI.pName = "#BSFC_simulink:1";
    ptr_gvar_instance->c2_p_emlrtRSI.lineNo = 12;
    ptr_gvar_instance->c2_p_emlrtRSI.fcnName = "evalScatteredData";
    ptr_gvar_instance->c2_p_emlrtRSI.pathName = "C:\\Program Files\\MATLAB\\R2025a\\toolbox\\shared\\coder\\coder\\+coder\\+internal\\@scatteredInterpolant\\evalScatteredData.m";
    ptr_gvar_instance->c2_q_emlrtRSI.lineNo = 14;
    ptr_gvar_instance->c2_q_emlrtRSI.fcnName = "linearInterpolationKernel";
    ptr_gvar_instance->c2_q_emlrtRSI.pathName = "C:\\Program Files\\MATLAB\\R2025a\\toolbox\\shared\\coder\\coder\\+coder\\+internal\\@scatteredInterpolant\\linearInterpolationK"
"ernel.m";
    ptr_gvar_instance->c2_r_emlrtRSI.lineNo = 26;
    ptr_gvar_instance->c2_r_emlrtRSI.fcnName = "linearInterpolationKernel";
    ptr_gvar_instance->c2_r_emlrtRSI.pathName = "C:\\Program Files\\MATLAB\\R2025a\\toolbox\\shared\\coder\\coder\\+coder\\+internal\\@scatteredInterpolant\\linearInterpolationK"
"ernel.m";
    ptr_gvar_instance->c2_s_emlrtRSI.lineNo = 30;
    ptr_gvar_instance->c2_s_emlrtRSI.fcnName = "linearInterpolationKernel";
    ptr_gvar_instance->c2_s_emlrtRSI.pathName = "C:\\Program Files\\MATLAB\\R2025a\\toolbox\\shared\\coder\\coder\\+coder\\+internal\\@scatteredInterpolant\\linearInterpolationK"
"ernel.m";
    ptr_gvar_instance->c2_t_emlrtRSI.lineNo = 35;
    ptr_gvar_instance->c2_t_emlrtRSI.fcnName = "linearInterpolationKernel";
    ptr_gvar_instance->c2_t_emlrtRSI.pathName = "C:\\Program Files\\MATLAB\\R2025a\\toolbox\\shared\\coder\\coder\\+coder\\+internal\\@scatteredInterpolant\\linearInterpolationK"
"ernel.m";
    ptr_gvar_instance->c2_u_emlrtRSI.lineNo = 56;
    ptr_gvar_instance->c2_u_emlrtRSI.fcnName = "linearInterpolationKernel";
    ptr_gvar_instance->c2_u_emlrtRSI.pathName = "C:\\Program Files\\MATLAB\\R2025a\\toolbox\\shared\\coder\\coder\\+coder\\+internal\\@scatteredInterpolant\\linearInterpolationK"
"ernel.m";
    ptr_gvar_instance->c2_v_emlrtRSI.lineNo = 73;
    ptr_gvar_instance->c2_v_emlrtRSI.fcnName = "delaunayTriangulation";
    ptr_gvar_instance->c2_v_emlrtRSI.pathName = "C:\\Program Files\\MATLAB\\R2025a\\toolbox\\shared\\coder\\coder\\+coder\\+internal\\@delaunayTriangulation\\delaunayTriangulati"
"on.m";
    ptr_gvar_instance->c2_w_emlrtRSI.lineNo = 136;
    ptr_gvar_instance->c2_w_emlrtRSI.fcnName = "delaunayTriangulation";
    ptr_gvar_instance->c2_w_emlrtRSI.pathName = "C:\\Program Files\\MATLAB\\R2025a\\toolbox\\shared\\coder\\coder\\+coder\\+internal\\@delaunayTriangulation\\delaunayTriangulati"
"on.m";
    ptr_gvar_instance->c2_x_emlrtRSI.lineNo = 41;
    ptr_gvar_instance->c2_x_emlrtRSI.fcnName = "solveBarycentricEqs";
    ptr_gvar_instance->c2_x_emlrtRSI.pathName = "C:\\Program Files\\MATLAB\\R2025a\\toolbox\\shared\\coder\\coder\\+coder\\+internal\\@scatteredInterpolant\\solveBarycentricEqs."
"m";
    ptr_gvar_instance->c2_y_emlrtRSI.lineNo = 20;
    ptr_gvar_instance->c2_y_emlrtRSI.fcnName = "mrdivide_helper";
    ptr_gvar_instance->c2_y_emlrtRSI.pathName = "C:\\Program Files\\MATLAB\\R2025a\\toolbox\\eml\\eml\\+coder\\+internal\\mrdivide_helper.m";
    return ptr_gvar_instance;
}


