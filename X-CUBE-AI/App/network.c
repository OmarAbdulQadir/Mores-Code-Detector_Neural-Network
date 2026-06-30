/**
  ******************************************************************************
  * @file    network.c
  * @author  AST Embedded Analytics Research Platform
  * @date    2026-06-30T16:30:04+0300
  * @brief   AI Tool Automatic Code Generator for Embedded NN computing
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  ******************************************************************************
  */


#include "network.h"
#include "network_data.h"

#include "ai_platform.h"
#include "ai_platform_interface.h"
#include "ai_math_helpers.h"

#include "core_common.h"
#include "core_convert.h"

#include "layers.h"



#undef AI_NET_OBJ_INSTANCE
#define AI_NET_OBJ_INSTANCE g_network
 
#undef AI_NETWORK_MODEL_SIGNATURE
#define AI_NETWORK_MODEL_SIGNATURE     "0x551416f368c22337767aced11646221e"

#ifndef AI_TOOLS_REVISION_ID
#define AI_TOOLS_REVISION_ID     ""
#endif

#undef AI_TOOLS_DATE_TIME
#define AI_TOOLS_DATE_TIME   "2026-06-30T16:30:04+0300"

#undef AI_TOOLS_COMPILE_TIME
#define AI_TOOLS_COMPILE_TIME    __DATE__ " " __TIME__

#undef AI_NETWORK_N_BATCHES
#define AI_NETWORK_N_BATCHES         (1)

static ai_ptr g_network_activations_map[1] = AI_C_ARRAY_INIT;
static ai_ptr g_network_weights_map[1] = AI_C_ARRAY_INIT;



/**  Array declarations section  **********************************************/
/* Array#0 */
AI_ARRAY_OBJ_DECLARE(
  serving_default_keras_tensor0_output_array, AI_ARRAY_FORMAT_S8|AI_FMT_FLAG_IS_IO,
  NULL, NULL, 100, AI_STATIC)

/* Array#1 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_1_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 1600, AI_STATIC)

/* Array#2 */
AI_ARRAY_OBJ_DECLARE(
  pool_4_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 800, AI_STATIC)

/* Array#3 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_7_pad_before_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 864, AI_STATIC)

/* Array#4 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_7_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 1600, AI_STATIC)

/* Array#5 */
AI_ARRAY_OBJ_DECLARE(
  pool_10_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 800, AI_STATIC)

/* Array#6 */
AI_ARRAY_OBJ_DECLARE(
  pool_12_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 32, AI_STATIC)

/* Array#7 */
AI_ARRAY_OBJ_DECLARE(
  gemm_13_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 32, AI_STATIC)

/* Array#8 */
AI_ARRAY_OBJ_DECLARE(
  gemm_14_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 28, AI_STATIC)

/* Array#9 */
AI_ARRAY_OBJ_DECLARE(
  nl_15_output_array, AI_ARRAY_FORMAT_S8|AI_FMT_FLAG_IS_IO,
  NULL, NULL, 28, AI_STATIC)

/* Array#10 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_1_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 80, AI_STATIC)

/* Array#11 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_1_bias_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 16, AI_STATIC)

/* Array#12 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_7_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 2560, AI_STATIC)

/* Array#13 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_7_bias_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 32, AI_STATIC)

/* Array#14 */
AI_ARRAY_OBJ_DECLARE(
  gemm_13_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 1024, AI_STATIC)

/* Array#15 */
AI_ARRAY_OBJ_DECLARE(
  gemm_13_bias_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 32, AI_STATIC)

/* Array#16 */
AI_ARRAY_OBJ_DECLARE(
  gemm_14_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 896, AI_STATIC)

/* Array#17 */
AI_ARRAY_OBJ_DECLARE(
  gemm_14_bias_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 28, AI_STATIC)

/* Array#18 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_1_scratch0_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 404, AI_STATIC)

/* Array#19 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_7_scratch0_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 5888, AI_STATIC)

/* Array#20 */
AI_ARRAY_OBJ_DECLARE(
  gemm_13_scratch0_array, AI_ARRAY_FORMAT_S16,
  NULL, NULL, 192, AI_STATIC)

/* Array#21 */
AI_ARRAY_OBJ_DECLARE(
  gemm_14_scratch0_array, AI_ARRAY_FORMAT_S16,
  NULL, NULL, 172, AI_STATIC)

/* Array#22 */
AI_ARRAY_OBJ_DECLARE(
  nl_15_scratch0_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 31, AI_STATIC)

/**  Array metadata declarations section  *************************************/
/* Int quant #0 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv2d_1_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.015141868032515049f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #1 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv2d_1_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 16,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.005866948049515486f, 0.008348006755113602f, 0.0018886999459937215f, 0.005370913073420525f, 0.01674892008304596f, 0.008981291204690933f, 0.014457803219556808f, 0.009006619453430176f, 0.016680853441357613f, 0.006047940347343683f, 0.008082193322479725f, 0.008332476951181889f, 0.005513851996511221f, 0.0014304236974567175f, 0.0061381300911307335f, 0.008351442404091358f),
    AI_PACK_INTQ_ZP(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)))

/* Int quant #2 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv2d_7_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.09568888694047928f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #3 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv2d_7_pad_before_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.015141868032515049f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #4 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv2d_7_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 32,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.011704863980412483f, 0.012235107831656933f, 0.008412723429501057f, 0.015865366905927658f, 0.00931848306208849f, 0.015648499131202698f, 0.010552135296165943f, 0.009964141063392162f, 0.008962897583842278f, 0.013439389877021313f, 0.0104768555611372f, 0.028925947844982147f, 0.0014733580173924565f, 0.011337975971400738f, 0.007474338635802269f, 0.010082579217851162f, 0.014148961752653122f, 0.03433169052004814f, 0.0012896595289930701f, 0.03472941741347313f, 0.008894408121705055f, 0.011039896868169308f, 0.00607018917798996f, 0.01110996026545763f, 0.00933249481022358f, 0.023198796436190605f, 0.024438658729195595f, 0.03403841704130173f, 0.0096753453835845f, 0.01365645695477724f, 0.01373140700161457f, 0.010208583436906338f),
    AI_PACK_INTQ_ZP(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)))

/* Int quant #5 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(gemm_13_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.06534094363451004f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #6 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(gemm_13_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 32,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0245033148676157f, 0.002300337655469775f, 0.010040463879704475f, 0.006037176586687565f, 0.0025649957824498415f, 0.002721205586567521f, 0.01154575776308775f, 0.002362338360399008f, 0.0024059289135038853f, 0.0025789791252464056f, 0.012231902219355106f, 0.021001258864998817f, 0.0025078521575778723f, 0.010624417103827f, 0.0150617565959692f, 0.008678872138261795f, 0.01683427207171917f, 0.030781973153352737f, 0.002376533579081297f, 0.0025715609081089497f, 0.013728749006986618f, 0.010281093418598175f, 0.020015109330415726f, 0.002458530943840742f, 0.015474438667297363f, 0.010275528766214848f, 0.017511805519461632f, 0.007288380526006222f, 0.0020808835979551077f, 0.0025872255209833384f, 0.010117928497493267f, 0.01875852234661579f),
    AI_PACK_INTQ_ZP(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)))

/* Int quant #7 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(gemm_14_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.9236869215965271f),
    AI_PACK_INTQ_ZP(102)))

/* Int quant #8 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(gemm_14_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 28,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.014334612525999546f, 0.02379920519888401f, 0.02256854623556137f, 0.017457259818911552f, 0.0180285032838583f, 0.020436905324459076f, 0.024570321664214134f, 0.02290992997586727f, 0.018317781388759613f, 0.024134524166584015f, 0.031049074605107307f, 0.017955303192138672f, 0.02750084176659584f, 0.019726432859897614f, 0.02072164975106716f, 0.02279609628021717f, 0.033877428621053696f, 0.026923762634396553f, 0.03212467581033707f, 0.028571033850312233f, 0.020828234031796455f, 0.017922362312674522f, 0.023785438388586044f, 0.0324941910803318f, 0.02575448527932167f, 0.025346070528030396f, 0.020411135628819466f, 0.02854309231042862f),
    AI_PACK_INTQ_ZP(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)))

/* Int quant #9 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(nl_15_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.00390625f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #10 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(pool_10_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.09568888694047928f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #11 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(pool_12_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.022019946947693825f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #12 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(pool_4_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.015141868032515049f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #13 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(serving_default_keras_tensor0_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.003921568859368563f),
    AI_PACK_INTQ_ZP(-128)))

/**  Tensor declarations section  *********************************************/
/* Tensor #0 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_1_bias, AI_STATIC,
  0, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 1, 1), AI_STRIDE_INIT(4, 4, 4, 64, 64),
  1, &conv2d_1_bias_array, NULL)

/* Tensor #1 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_1_output, AI_STATIC,
  1, 0x1,
  AI_SHAPE_INIT(4, 1, 16, 100, 1), AI_STRIDE_INIT(4, 1, 1, 16, 1600),
  1, &conv2d_1_output_array, &conv2d_1_output_array_intq)

/* Tensor #2 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_1_scratch0, AI_STATIC,
  2, 0x0,
  AI_SHAPE_INIT(4, 1, 404, 1, 1), AI_STRIDE_INIT(4, 1, 1, 404, 404),
  1, &conv2d_1_scratch0_array, NULL)

/* Tensor #3 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_1_weights, AI_STATIC,
  3, 0x1,
  AI_SHAPE_INIT(4, 1, 5, 1, 16), AI_STRIDE_INIT(4, 1, 1, 16, 80),
  1, &conv2d_1_weights_array, &conv2d_1_weights_array_intq)

/* Tensor #4 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_7_bias, AI_STATIC,
  4, 0x0,
  AI_SHAPE_INIT(4, 1, 32, 1, 1), AI_STRIDE_INIT(4, 4, 4, 128, 128),
  1, &conv2d_7_bias_array, NULL)

/* Tensor #5 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_7_output, AI_STATIC,
  5, 0x1,
  AI_SHAPE_INIT(4, 1, 32, 50, 1), AI_STRIDE_INIT(4, 1, 1, 32, 1600),
  1, &conv2d_7_output_array, &conv2d_7_output_array_intq)

/* Tensor #6 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_7_pad_before_output, AI_STATIC,
  6, 0x1,
  AI_SHAPE_INIT(4, 1, 16, 54, 1), AI_STRIDE_INIT(4, 1, 1, 16, 864),
  1, &conv2d_7_pad_before_output_array, &conv2d_7_pad_before_output_array_intq)

/* Tensor #7 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_7_scratch0, AI_STATIC,
  7, 0x0,
  AI_SHAPE_INIT(4, 1, 5888, 1, 1), AI_STRIDE_INIT(4, 1, 1, 5888, 5888),
  1, &conv2d_7_scratch0_array, NULL)

/* Tensor #8 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_7_weights, AI_STATIC,
  8, 0x1,
  AI_SHAPE_INIT(4, 16, 5, 1, 32), AI_STRIDE_INIT(4, 1, 16, 512, 2560),
  1, &conv2d_7_weights_array, &conv2d_7_weights_array_intq)

/* Tensor #9 */
AI_TENSOR_OBJ_DECLARE(
  gemm_13_bias, AI_STATIC,
  9, 0x0,
  AI_SHAPE_INIT(4, 1, 32, 1, 1), AI_STRIDE_INIT(4, 4, 4, 128, 128),
  1, &gemm_13_bias_array, NULL)

/* Tensor #10 */
AI_TENSOR_OBJ_DECLARE(
  gemm_13_output, AI_STATIC,
  10, 0x1,
  AI_SHAPE_INIT(4, 1, 32, 1, 1), AI_STRIDE_INIT(4, 1, 1, 32, 32),
  1, &gemm_13_output_array, &gemm_13_output_array_intq)

/* Tensor #11 */
AI_TENSOR_OBJ_DECLARE(
  gemm_13_scratch0, AI_STATIC,
  11, 0x0,
  AI_SHAPE_INIT(4, 1, 192, 1, 1), AI_STRIDE_INIT(4, 2, 2, 384, 384),
  1, &gemm_13_scratch0_array, NULL)

/* Tensor #12 */
AI_TENSOR_OBJ_DECLARE(
  gemm_13_weights, AI_STATIC,
  12, 0x1,
  AI_SHAPE_INIT(4, 32, 32, 1, 1), AI_STRIDE_INIT(4, 1, 32, 1024, 1024),
  1, &gemm_13_weights_array, &gemm_13_weights_array_intq)

/* Tensor #13 */
AI_TENSOR_OBJ_DECLARE(
  gemm_14_bias, AI_STATIC,
  13, 0x0,
  AI_SHAPE_INIT(4, 1, 28, 1, 1), AI_STRIDE_INIT(4, 4, 4, 112, 112),
  1, &gemm_14_bias_array, NULL)

/* Tensor #14 */
AI_TENSOR_OBJ_DECLARE(
  gemm_14_output, AI_STATIC,
  14, 0x1,
  AI_SHAPE_INIT(4, 1, 28, 1, 1), AI_STRIDE_INIT(4, 1, 1, 28, 28),
  1, &gemm_14_output_array, &gemm_14_output_array_intq)

/* Tensor #15 */
AI_TENSOR_OBJ_DECLARE(
  gemm_14_scratch0, AI_STATIC,
  15, 0x0,
  AI_SHAPE_INIT(4, 1, 172, 1, 1), AI_STRIDE_INIT(4, 2, 2, 344, 344),
  1, &gemm_14_scratch0_array, NULL)

/* Tensor #16 */
AI_TENSOR_OBJ_DECLARE(
  gemm_14_weights, AI_STATIC,
  16, 0x1,
  AI_SHAPE_INIT(4, 32, 28, 1, 1), AI_STRIDE_INIT(4, 1, 32, 896, 896),
  1, &gemm_14_weights_array, &gemm_14_weights_array_intq)

/* Tensor #17 */
AI_TENSOR_OBJ_DECLARE(
  nl_15_output, AI_STATIC,
  17, 0x1,
  AI_SHAPE_INIT(4, 1, 28, 1, 1), AI_STRIDE_INIT(4, 1, 1, 28, 28),
  1, &nl_15_output_array, &nl_15_output_array_intq)

/* Tensor #18 */
AI_TENSOR_OBJ_DECLARE(
  nl_15_scratch0, AI_STATIC,
  18, 0x0,
  AI_SHAPE_INIT(4, 1, 31, 1, 1), AI_STRIDE_INIT(4, 4, 4, 124, 124),
  1, &nl_15_scratch0_array, NULL)

/* Tensor #19 */
AI_TENSOR_OBJ_DECLARE(
  pool_10_output, AI_STATIC,
  19, 0x1,
  AI_SHAPE_INIT(4, 1, 32, 25, 1), AI_STRIDE_INIT(4, 1, 1, 32, 800),
  1, &pool_10_output_array, &pool_10_output_array_intq)

/* Tensor #20 */
AI_TENSOR_OBJ_DECLARE(
  pool_10_output0, AI_STATIC,
  20, 0x1,
  AI_SHAPE_INIT(4, 1, 32, 1, 25), AI_STRIDE_INIT(4, 1, 1, 32, 32),
  1, &pool_10_output_array, &pool_10_output_array_intq)

/* Tensor #21 */
AI_TENSOR_OBJ_DECLARE(
  pool_12_output, AI_STATIC,
  21, 0x1,
  AI_SHAPE_INIT(4, 1, 32, 1, 1), AI_STRIDE_INIT(4, 1, 1, 32, 32),
  1, &pool_12_output_array, &pool_12_output_array_intq)

/* Tensor #22 */
AI_TENSOR_OBJ_DECLARE(
  pool_4_output, AI_STATIC,
  22, 0x1,
  AI_SHAPE_INIT(4, 1, 16, 50, 1), AI_STRIDE_INIT(4, 1, 1, 16, 800),
  1, &pool_4_output_array, &pool_4_output_array_intq)

/* Tensor #23 */
AI_TENSOR_OBJ_DECLARE(
  serving_default_keras_tensor0_output, AI_STATIC,
  23, 0x1,
  AI_SHAPE_INIT(4, 1, 1, 1, 100), AI_STRIDE_INIT(4, 1, 1, 1, 1),
  1, &serving_default_keras_tensor0_output_array, &serving_default_keras_tensor0_output_array_intq)

/* Tensor #24 */
AI_TENSOR_OBJ_DECLARE(
  serving_default_keras_tensor0_output0, AI_STATIC,
  24, 0x1,
  AI_SHAPE_INIT(4, 1, 1, 100, 1), AI_STRIDE_INIT(4, 1, 1, 1, 100),
  1, &serving_default_keras_tensor0_output_array, &serving_default_keras_tensor0_output_array_intq)



/**  Layer declarations section  **********************************************/



AI_STATIC_CONST ai_i32 nl_15_nl_params_data[] = { 1983602560, 26, -31 };
AI_ARRAY_OBJ_DECLARE(
    nl_15_nl_params, AI_ARRAY_FORMAT_S32,
    nl_15_nl_params_data, nl_15_nl_params_data, 3, AI_STATIC_CONST)
AI_TENSOR_CHAIN_OBJ_DECLARE(
  nl_15_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_14_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &nl_15_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &nl_15_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  nl_15_layer, 15,
  SM_TYPE, 0x0, NULL,
  sm, forward_sm_integer,
  &nl_15_chain,
  NULL, &nl_15_layer, AI_STATIC, 
  .nl_params = &nl_15_nl_params, 
  .axis = AI_SHAPE_CHANNEL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  gemm_14_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_13_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_14_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &gemm_14_weights, &gemm_14_bias),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_14_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  gemm_14_layer, 14,
  DENSE_TYPE, 0x0, NULL,
  dense, forward_dense_integer_SSSA_ch,
  &gemm_14_chain,
  NULL, &nl_15_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  gemm_13_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &pool_12_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_13_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &gemm_13_weights, &gemm_13_bias),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_13_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  gemm_13_layer, 13,
  DENSE_TYPE, 0x0, NULL,
  dense, forward_dense_integer_SSSA_ch,
  &gemm_13_chain,
  NULL, &gemm_14_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  pool_12_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &pool_10_output0),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &pool_12_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  pool_12_layer, 12,
  POOL_TYPE, 0x0, NULL,
  pool, forward_ap_integer_INT8,
  &pool_12_chain,
  NULL, &gemm_13_layer, AI_STATIC, 
  .pool_size = AI_SHAPE_2D_INIT(1, 25), 
  .pool_stride = AI_SHAPE_2D_INIT(1, 25), 
  .pool_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  pool_10_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_7_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &pool_10_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  pool_10_layer, 10,
  POOL_TYPE, 0x0, NULL,
  pool, forward_mp_integer_INT8,
  &pool_10_chain,
  NULL, &pool_12_layer, AI_STATIC, 
  .pool_size = AI_SHAPE_2D_INIT(2, 1), 
  .pool_stride = AI_SHAPE_2D_INIT(2, 1), 
  .pool_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv2d_7_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_7_pad_before_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_7_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &conv2d_7_weights, &conv2d_7_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_7_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  conv2d_7_layer, 7,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_deep_sssa8_ch,
  &conv2d_7_chain,
  NULL, &pool_10_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)


AI_STATIC_CONST ai_i8 conv2d_7_pad_before_value_data[] = { -128 };
AI_ARRAY_OBJ_DECLARE(
    conv2d_7_pad_before_value, AI_ARRAY_FORMAT_S8,
    conv2d_7_pad_before_value_data, conv2d_7_pad_before_value_data, 1, AI_STATIC_CONST)
AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv2d_7_pad_before_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &pool_4_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_7_pad_before_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv2d_7_pad_before_layer, 7,
  PAD_TYPE, 0x0, NULL,
  pad, forward_pad,
  &conv2d_7_pad_before_chain,
  NULL, &conv2d_7_layer, AI_STATIC, 
  .value = &conv2d_7_pad_before_value, 
  .mode = AI_PAD_CONSTANT, 
  .pads = AI_SHAPE_INIT(4, 0, 2, 0, 2), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  pool_4_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_1_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &pool_4_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  pool_4_layer, 4,
  POOL_TYPE, 0x0, NULL,
  pool, forward_mp_integer_INT8,
  &pool_4_chain,
  NULL, &conv2d_7_pad_before_layer, AI_STATIC, 
  .pool_size = AI_SHAPE_2D_INIT(2, 1), 
  .pool_stride = AI_SHAPE_2D_INIT(2, 1), 
  .pool_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv2d_1_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &serving_default_keras_tensor0_output0),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_1_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &conv2d_1_weights, &conv2d_1_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_1_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  conv2d_1_layer, 1,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_sssa8_ch,
  &conv2d_1_chain,
  NULL, &pool_4_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 2, 0, 2), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)


#if (AI_TOOLS_API_VERSION < AI_TOOLS_API_VERSION_1_5)

AI_NETWORK_OBJ_DECLARE(
  AI_NET_OBJ_INSTANCE, AI_STATIC,
  AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
    AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 4992, 1, 1),
    4992, NULL, NULL),
  AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
    AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 8352, 1, 1),
    8352, NULL, NULL),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_NETWORK_IN_NUM, &serving_default_keras_tensor0_output),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_NETWORK_OUT_NUM, &nl_15_output),
  &conv2d_1_layer, 0x05a06f0e, NULL)

#else

AI_NETWORK_OBJ_DECLARE(
  AI_NET_OBJ_INSTANCE, AI_STATIC,
  AI_BUFFER_ARRAY_OBJ_INIT_STATIC(
  	AI_FLAG_NONE, 1,
    AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
      AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 4992, 1, 1),
      4992, NULL, NULL)
  ),
  AI_BUFFER_ARRAY_OBJ_INIT_STATIC(
  	AI_FLAG_NONE, 1,
    AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
      AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 8352, 1, 1),
      8352, NULL, NULL)
  ),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_NETWORK_IN_NUM, &serving_default_keras_tensor0_output),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_NETWORK_OUT_NUM, &nl_15_output),
  &conv2d_1_layer, 0x05a06f0e, NULL)

#endif	/*(AI_TOOLS_API_VERSION < AI_TOOLS_API_VERSION_1_5)*/



/******************************************************************************/
AI_DECLARE_STATIC
ai_bool network_configure_activations(
  ai_network* net_ctx, const ai_network_params* params)
{
  AI_ASSERT(net_ctx)

  if (ai_platform_get_activations_map(g_network_activations_map, 1, params)) {
    /* Updating activations (byte) offsets */
    
    serving_default_keras_tensor0_output_array.data = AI_PTR(g_network_activations_map[0] + 1160);
    serving_default_keras_tensor0_output_array.data_start = AI_PTR(g_network_activations_map[0] + 1160);
    conv2d_1_scratch0_array.data = AI_PTR(g_network_activations_map[0] + 1260);
    conv2d_1_scratch0_array.data_start = AI_PTR(g_network_activations_map[0] + 1260);
    conv2d_1_output_array.data = AI_PTR(g_network_activations_map[0] + 1664);
    conv2d_1_output_array.data_start = AI_PTR(g_network_activations_map[0] + 1664);
    pool_4_output_array.data = AI_PTR(g_network_activations_map[0] + 1664);
    pool_4_output_array.data_start = AI_PTR(g_network_activations_map[0] + 1664);
    conv2d_7_pad_before_output_array.data = AI_PTR(g_network_activations_map[0] + 1600);
    conv2d_7_pad_before_output_array.data_start = AI_PTR(g_network_activations_map[0] + 1600);
    conv2d_7_scratch0_array.data = AI_PTR(g_network_activations_map[0] + 2464);
    conv2d_7_scratch0_array.data_start = AI_PTR(g_network_activations_map[0] + 2464);
    conv2d_7_output_array.data = AI_PTR(g_network_activations_map[0] + 0);
    conv2d_7_output_array.data_start = AI_PTR(g_network_activations_map[0] + 0);
    pool_10_output_array.data = AI_PTR(g_network_activations_map[0] + 1600);
    pool_10_output_array.data_start = AI_PTR(g_network_activations_map[0] + 1600);
    pool_12_output_array.data = AI_PTR(g_network_activations_map[0] + 0);
    pool_12_output_array.data_start = AI_PTR(g_network_activations_map[0] + 0);
    gemm_13_scratch0_array.data = AI_PTR(g_network_activations_map[0] + 32);
    gemm_13_scratch0_array.data_start = AI_PTR(g_network_activations_map[0] + 32);
    gemm_13_output_array.data = AI_PTR(g_network_activations_map[0] + 416);
    gemm_13_output_array.data_start = AI_PTR(g_network_activations_map[0] + 416);
    gemm_14_scratch0_array.data = AI_PTR(g_network_activations_map[0] + 0);
    gemm_14_scratch0_array.data_start = AI_PTR(g_network_activations_map[0] + 0);
    gemm_14_output_array.data = AI_PTR(g_network_activations_map[0] + 344);
    gemm_14_output_array.data_start = AI_PTR(g_network_activations_map[0] + 344);
    nl_15_scratch0_array.data = AI_PTR(g_network_activations_map[0] + 0);
    nl_15_scratch0_array.data_start = AI_PTR(g_network_activations_map[0] + 0);
    nl_15_output_array.data = AI_PTR(g_network_activations_map[0] + 124);
    nl_15_output_array.data_start = AI_PTR(g_network_activations_map[0] + 124);
    return true;
  }
  AI_ERROR_TRAP(net_ctx, INIT_FAILED, NETWORK_ACTIVATIONS);
  return false;
}




/******************************************************************************/
AI_DECLARE_STATIC
ai_bool network_configure_weights(
  ai_network* net_ctx, const ai_network_params* params)
{
  AI_ASSERT(net_ctx)

  if (ai_platform_get_weights_map(g_network_weights_map, 1, params)) {
    /* Updating weights (byte) offsets */
    
    conv2d_1_weights_array.format |= AI_FMT_FLAG_CONST;
    conv2d_1_weights_array.data = AI_PTR(g_network_weights_map[0] + 0);
    conv2d_1_weights_array.data_start = AI_PTR(g_network_weights_map[0] + 0);
    conv2d_1_bias_array.format |= AI_FMT_FLAG_CONST;
    conv2d_1_bias_array.data = AI_PTR(g_network_weights_map[0] + 80);
    conv2d_1_bias_array.data_start = AI_PTR(g_network_weights_map[0] + 80);
    conv2d_7_weights_array.format |= AI_FMT_FLAG_CONST;
    conv2d_7_weights_array.data = AI_PTR(g_network_weights_map[0] + 144);
    conv2d_7_weights_array.data_start = AI_PTR(g_network_weights_map[0] + 144);
    conv2d_7_bias_array.format |= AI_FMT_FLAG_CONST;
    conv2d_7_bias_array.data = AI_PTR(g_network_weights_map[0] + 2704);
    conv2d_7_bias_array.data_start = AI_PTR(g_network_weights_map[0] + 2704);
    gemm_13_weights_array.format |= AI_FMT_FLAG_CONST;
    gemm_13_weights_array.data = AI_PTR(g_network_weights_map[0] + 2832);
    gemm_13_weights_array.data_start = AI_PTR(g_network_weights_map[0] + 2832);
    gemm_13_bias_array.format |= AI_FMT_FLAG_CONST;
    gemm_13_bias_array.data = AI_PTR(g_network_weights_map[0] + 3856);
    gemm_13_bias_array.data_start = AI_PTR(g_network_weights_map[0] + 3856);
    gemm_14_weights_array.format |= AI_FMT_FLAG_CONST;
    gemm_14_weights_array.data = AI_PTR(g_network_weights_map[0] + 3984);
    gemm_14_weights_array.data_start = AI_PTR(g_network_weights_map[0] + 3984);
    gemm_14_bias_array.format |= AI_FMT_FLAG_CONST;
    gemm_14_bias_array.data = AI_PTR(g_network_weights_map[0] + 4880);
    gemm_14_bias_array.data_start = AI_PTR(g_network_weights_map[0] + 4880);
    return true;
  }
  AI_ERROR_TRAP(net_ctx, INIT_FAILED, NETWORK_WEIGHTS);
  return false;
}


/**  PUBLIC APIs SECTION  *****************************************************/



AI_DEPRECATED
AI_API_ENTRY
ai_bool ai_network_get_info(
  ai_handle network, ai_network_report* report)
{
  ai_network* net_ctx = AI_NETWORK_ACQUIRE_CTX(network);

  if (report && net_ctx)
  {
    ai_network_report r = {
      .model_name        = AI_NETWORK_MODEL_NAME,
      .model_signature   = AI_NETWORK_MODEL_SIGNATURE,
      .model_datetime    = AI_TOOLS_DATE_TIME,
      
      .compile_datetime  = AI_TOOLS_COMPILE_TIME,
      
      .runtime_revision  = ai_platform_runtime_get_revision(),
      .runtime_version   = ai_platform_runtime_get_version(),

      .tool_revision     = AI_TOOLS_REVISION_ID,
      .tool_version      = {AI_TOOLS_VERSION_MAJOR, AI_TOOLS_VERSION_MINOR,
                            AI_TOOLS_VERSION_MICRO, 0x0},
      .tool_api_version  = AI_STRUCT_INIT,

      .api_version            = ai_platform_api_get_version(),
      .interface_api_version  = ai_platform_interface_api_get_version(),
      
      .n_macc            = 142448,
      .n_inputs          = 0,
      .inputs            = NULL,
      .n_outputs         = 0,
      .outputs           = NULL,
      .params            = AI_STRUCT_INIT,
      .activations       = AI_STRUCT_INIT,
      .n_nodes           = 0,
      .signature         = 0x05a06f0e,
    };

    if (!ai_platform_api_get_network_report(network, &r)) return false;

    *report = r;
    return true;
  }
  return false;
}



AI_API_ENTRY
ai_bool ai_network_get_report(
  ai_handle network, ai_network_report* report)
{
  ai_network* net_ctx = AI_NETWORK_ACQUIRE_CTX(network);

  if (report && net_ctx)
  {
    ai_network_report r = {
      .model_name        = AI_NETWORK_MODEL_NAME,
      .model_signature   = AI_NETWORK_MODEL_SIGNATURE,
      .model_datetime    = AI_TOOLS_DATE_TIME,
      
      .compile_datetime  = AI_TOOLS_COMPILE_TIME,
      
      .runtime_revision  = ai_platform_runtime_get_revision(),
      .runtime_version   = ai_platform_runtime_get_version(),

      .tool_revision     = AI_TOOLS_REVISION_ID,
      .tool_version      = {AI_TOOLS_VERSION_MAJOR, AI_TOOLS_VERSION_MINOR,
                            AI_TOOLS_VERSION_MICRO, 0x0},
      .tool_api_version  = AI_STRUCT_INIT,

      .api_version            = ai_platform_api_get_version(),
      .interface_api_version  = ai_platform_interface_api_get_version(),
      
      .n_macc            = 142448,
      .n_inputs          = 0,
      .inputs            = NULL,
      .n_outputs         = 0,
      .outputs           = NULL,
      .map_signature     = AI_MAGIC_SIGNATURE,
      .map_weights       = AI_STRUCT_INIT,
      .map_activations   = AI_STRUCT_INIT,
      .n_nodes           = 0,
      .signature         = 0x05a06f0e,
    };

    if (!ai_platform_api_get_network_report(network, &r)) return false;

    *report = r;
    return true;
  }
  return false;
}


AI_API_ENTRY
ai_error ai_network_get_error(ai_handle network)
{
  return ai_platform_network_get_error(network);
}


AI_API_ENTRY
ai_error ai_network_create(
  ai_handle* network, const ai_buffer* network_config)
{
  return ai_platform_network_create(
    network, network_config, 
    AI_CONTEXT_OBJ(&AI_NET_OBJ_INSTANCE),
    AI_TOOLS_API_VERSION_MAJOR, AI_TOOLS_API_VERSION_MINOR, AI_TOOLS_API_VERSION_MICRO);
}


AI_API_ENTRY
ai_error ai_network_create_and_init(
  ai_handle* network, const ai_handle activations[], const ai_handle weights[])
{
  ai_error err;
  ai_network_params params;

  err = ai_network_create(network, AI_NETWORK_DATA_CONFIG);
  if (err.type != AI_ERROR_NONE) {
    return err;
  }
  
  if (ai_network_data_params_get(&params) != true) {
    err = ai_network_get_error(*network);
    return err;
  }
#if defined(AI_NETWORK_DATA_ACTIVATIONS_COUNT)
  /* set the addresses of the activations buffers */
  for (ai_u16 idx=0; activations && idx<params.map_activations.size; idx++) {
    AI_BUFFER_ARRAY_ITEM_SET_ADDRESS(&params.map_activations, idx, activations[idx]);
  }
#endif
#if defined(AI_NETWORK_DATA_WEIGHTS_COUNT)
  /* set the addresses of the weight buffers */
  for (ai_u16 idx=0; weights && idx<params.map_weights.size; idx++) {
    AI_BUFFER_ARRAY_ITEM_SET_ADDRESS(&params.map_weights, idx, weights[idx]);
  }
#endif
  if (ai_network_init(*network, &params) != true) {
    err = ai_network_get_error(*network);
  }
  return err;
}


AI_API_ENTRY
ai_buffer* ai_network_inputs_get(ai_handle network, ai_u16 *n_buffer)
{
  if (network == AI_HANDLE_NULL) {
    network = (ai_handle)&AI_NET_OBJ_INSTANCE;
    AI_NETWORK_OBJ(network)->magic = AI_MAGIC_CONTEXT_TOKEN;
  }
  return ai_platform_inputs_get(network, n_buffer);
}


AI_API_ENTRY
ai_buffer* ai_network_outputs_get(ai_handle network, ai_u16 *n_buffer)
{
  if (network == AI_HANDLE_NULL) {
    network = (ai_handle)&AI_NET_OBJ_INSTANCE;
    AI_NETWORK_OBJ(network)->magic = AI_MAGIC_CONTEXT_TOKEN;
  }
  return ai_platform_outputs_get(network, n_buffer);
}


AI_API_ENTRY
ai_handle ai_network_destroy(ai_handle network)
{
  return ai_platform_network_destroy(network);
}


AI_API_ENTRY
ai_bool ai_network_init(
  ai_handle network, const ai_network_params* params)
{
  ai_network* net_ctx = AI_NETWORK_OBJ(ai_platform_network_init(network, params));
  ai_bool ok = true;

  if (!net_ctx) return false;
  ok &= network_configure_weights(net_ctx, params);
  ok &= network_configure_activations(net_ctx, params);

  ok &= ai_platform_network_post_init(network);

  return ok;
}


AI_API_ENTRY
ai_i32 ai_network_run(
  ai_handle network, const ai_buffer* input, ai_buffer* output)
{
  return ai_platform_network_process(network, input, output);
}


AI_API_ENTRY
ai_i32 ai_network_forward(ai_handle network, const ai_buffer* input)
{
  return ai_platform_network_process(network, input, NULL);
}



#undef AI_NETWORK_MODEL_SIGNATURE
#undef AI_NET_OBJ_INSTANCE
#undef AI_TOOLS_DATE_TIME
#undef AI_TOOLS_COMPILE_TIME

