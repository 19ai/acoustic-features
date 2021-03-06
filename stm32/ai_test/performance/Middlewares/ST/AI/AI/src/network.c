/**
  ******************************************************************************
  * @file    network.c
  * @author  AST Embedded Analytics Research Platform
  * @date    Thu Mar 21 20:12:28 2019
  * @brief   AI Tool Automatic Code Generator for Embedded NN computing
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */



#include "network.h"

#include "ai_platform_interface.h"
#include "ai_math_helpers.h"

#include "core_common.h"
#include "layers.h"

#undef AI_TOOLS_VERSION_MAJOR
#undef AI_TOOLS_VERSION_MINOR
#undef AI_TOOLS_VERSION_MICRO
#define AI_TOOLS_VERSION_MAJOR 3
#define AI_TOOLS_VERSION_MINOR 3
#define AI_TOOLS_VERSION_MICRO 0

#undef AI_TOOLS_API_VERSION_MAJOR
#undef AI_TOOLS_API_VERSION_MINOR
#undef AI_TOOLS_API_VERSION_MICRO
#define AI_TOOLS_API_VERSION_MAJOR 1
#define AI_TOOLS_API_VERSION_MINOR 1
#define AI_TOOLS_API_VERSION_MICRO 0

#undef AI_NET_OBJ_INSTANCE
#define AI_NET_OBJ_INSTANCE g_network
 
#undef AI_NETWORK_MODEL_SIGNATURE
#define AI_NETWORK_MODEL_SIGNATURE     "c52453b0b76db4fd700affe763012d75"

#ifndef AI_TOOLS_REVISION_ID
#define AI_TOOLS_REVISION_ID     "(rev-)"
#endif

#undef AI_TOOLS_DATE_TIME
#define AI_TOOLS_DATE_TIME   "Thu Mar 21 20:12:28 2019"

#undef AI_TOOLS_COMPILE_TIME
#define AI_TOOLS_COMPILE_TIME    __DATE__ " " __TIME__

#undef AI_NETWORK_N_BATCHES
#define AI_NETWORK_N_BATCHES         (1)

/**  Forward network declaration section  *************************************/
AI_STATIC ai_network AI_NET_OBJ_INSTANCE;


/**  Forward network arrays declarations  *************************************/
AI_STATIC ai_array input_0_output_array;   /* Array #0 */
AI_STATIC ai_array conv2d_59_output_array;   /* Array #1 */
AI_STATIC ai_array conv2d_60_output_array;   /* Array #2 */
AI_STATIC ai_array conv2d_61_output_in_array;   /* Array #3 */
AI_STATIC ai_array conv2d_61_output_out_array;   /* Array #4 */
AI_STATIC ai_array dense_33_output_array;   /* Array #5 */
AI_STATIC ai_array dense_33_nl_output_array;   /* Array #6 */
AI_STATIC ai_array dense_34_output_array;   /* Array #7 */
AI_STATIC ai_array dense_34_nl_output_array;   /* Array #8 */


/**  Forward network tensors declarations  ************************************/
AI_STATIC ai_tensor input_0_output;   /* Tensor #0 */
AI_STATIC ai_tensor conv2d_59_output;   /* Tensor #1 */
AI_STATIC ai_tensor conv2d_60_output;   /* Tensor #2 */
AI_STATIC ai_tensor conv2d_61_output_in;   /* Tensor #3 */
AI_STATIC ai_tensor conv2d_61_output_out;   /* Tensor #4 */
AI_STATIC ai_tensor dense_33_output;   /* Tensor #5 */
AI_STATIC ai_tensor dense_33_nl_output;   /* Tensor #6 */
AI_STATIC ai_tensor dense_34_output;   /* Tensor #7 */
AI_STATIC ai_tensor dense_34_nl_output;   /* Tensor #8 */


/**  Forward network tensor chain declarations  *******************************/
AI_STATIC_CONST ai_tensor_chain conv2d_59_chain;   /* Chain #0 */
AI_STATIC_CONST ai_tensor_chain conv2d_60_chain;   /* Chain #1 */
AI_STATIC_CONST ai_tensor_chain conv2d_61_chain;   /* Chain #2 */
AI_STATIC_CONST ai_tensor_chain dense_33_chain;   /* Chain #3 */
AI_STATIC_CONST ai_tensor_chain dense_33_nl_chain;   /* Chain #4 */
AI_STATIC_CONST ai_tensor_chain dense_34_chain;   /* Chain #5 */
AI_STATIC_CONST ai_tensor_chain dense_34_nl_chain;   /* Chain #6 */


/**  Subgraph network operators tensor chain declarations  *********************************/


/**  Subgraph network operators declarations  *********************************/


/**  Forward network layers declarations  *************************************/
AI_STATIC ai_layer_conv2d_nl_pool conv2d_59_layer; /* Layer #0 */
AI_STATIC ai_layer_conv2d_nl_pool conv2d_60_layer; /* Layer #1 */
AI_STATIC ai_layer_conv2d_nl_pool conv2d_61_layer; /* Layer #2 */
AI_STATIC ai_layer_dense dense_33_layer; /* Layer #3 */
AI_STATIC ai_layer_nl dense_33_nl_layer; /* Layer #4 */
AI_STATIC ai_layer_dense dense_34_layer; /* Layer #5 */
AI_STATIC ai_layer_sm dense_34_nl_layer; /* Layer #6 */


/**  Arrays declarations section  *********************************************/
AI_ARRAY_OBJ_DECLARE(
  input_0_output_array, AI_DATA_FORMAT_FLOAT, 
  NULL, NULL, 2560,
  AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
  conv2d_59_output_array, AI_DATA_FORMAT_FLOAT, 
  NULL, NULL, 4712,
  AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
  conv2d_60_output_array, AI_DATA_FORMAT_FLOAT, 
  NULL, NULL, 1792,
  AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
  conv2d_61_output_in_array, AI_DATA_FORMAT_FLOAT, 
  NULL, NULL, 576,
  AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
  conv2d_61_output_out_array, AI_DATA_FORMAT_FLOAT, 
  NULL, NULL, 576,
  AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
  dense_33_output_array, AI_DATA_FORMAT_FLOAT, 
  NULL, NULL, 128,
  AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
  dense_33_nl_output_array, AI_DATA_FORMAT_FLOAT, 
  NULL, NULL, 128,
  AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
  dense_34_output_array, AI_DATA_FORMAT_FLOAT, 
  NULL, NULL, 10,
  AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
  dense_34_nl_output_array, AI_DATA_FORMAT_FLOAT, 
  NULL, NULL, 10,
  AI_STATIC)


/**  Activations tensors declaration section  *********************************/
AI_TENSOR_OBJ_DECLARE(
  input_0_output,
  AI_SHAPE_INIT(64, 40, 1, 1),
  AI_STRIDE_INIT(160, 4, 4, 4),
  &input_0_output_array,
  AI_STATIC)
AI_TENSOR_OBJ_DECLARE(
  conv2d_59_output,
  AI_SHAPE_INIT(31, 19, 8, 1),
  AI_STRIDE_INIT(608, 32, 4, 4),
  &conv2d_59_output_array,
  AI_STATIC)
AI_TENSOR_OBJ_DECLARE(
  conv2d_60_output,
  AI_SHAPE_INIT(14, 8, 16, 1),
  AI_STRIDE_INIT(512, 64, 4, 4),
  &conv2d_60_output_array,
  AI_STATIC)
AI_TENSOR_OBJ_DECLARE(
  conv2d_61_output_in,
  AI_SHAPE_INIT(6, 3, 32, 1),
  AI_STRIDE_INIT(384, 128, 4, 4),
  &conv2d_61_output_in_array,
  AI_STATIC)
AI_TENSOR_OBJ_DECLARE(
  conv2d_61_output_out,
  AI_SHAPE_INIT(1, 1, 576, 1),
  AI_STRIDE_INIT(2304, 2304, 4, 4),
  &conv2d_61_output_out_array,
  AI_STATIC)
AI_TENSOR_OBJ_DECLARE(
  dense_33_output,
  AI_SHAPE_INIT(1, 1, 128, 1),
  AI_STRIDE_INIT(512, 512, 4, 4),
  &dense_33_output_array,
  AI_STATIC)
AI_TENSOR_OBJ_DECLARE(
  dense_33_nl_output,
  AI_SHAPE_INIT(1, 1, 128, 1),
  AI_STRIDE_INIT(512, 512, 4, 4),
  &dense_33_nl_output_array,
  AI_STATIC)
AI_TENSOR_OBJ_DECLARE(
  dense_34_output,
  AI_SHAPE_INIT(1, 1, 10, 1),
  AI_STRIDE_INIT(40, 40, 4, 4),
  &dense_34_output_array,
  AI_STATIC)
AI_TENSOR_OBJ_DECLARE(
  dense_34_nl_output,
  AI_SHAPE_INIT(1, 1, 10, 1),
  AI_STRIDE_INIT(40, 40, 4, 4),
  &dense_34_nl_output_array,
  AI_STATIC)





/* Layer #0: "conv2d_59" (Conv2D) */
  

/* Weight tensor #1 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_59_weights_array, AI_DATA_FORMAT_FLOAT, 
  NULL, NULL, 72,
  AI_STATIC)

AI_TENSOR_OBJ_DECLARE(
  conv2d_59_weights,
  AI_SHAPE_INIT(8, 3, 3, 1),
  AI_STRIDE_INIT(36, 12, 4, 4),
  &conv2d_59_weights_array,
  AI_STATIC)

/* Weight tensor #2 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_59_bias_array, AI_DATA_FORMAT_FLOAT, 
  NULL, NULL, 8,
  AI_STATIC)

AI_TENSOR_OBJ_DECLARE(
  conv2d_59_bias,
  AI_SHAPE_INIT(1, 1, 8, 1),
  AI_STRIDE_INIT(32, 32, 4, 4),
  &conv2d_59_bias_array,
  AI_STATIC)


AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv2d_59_chain, AI_STATIC_CONST, 
  AI_TENSOR_LIST_ENTRY(&input_0_output),
  AI_TENSOR_LIST_ENTRY(&conv2d_59_output),
  AI_TENSOR_LIST_ENTRY(&conv2d_59_weights, &conv2d_59_bias),
  AI_TENSOR_LIST_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv2d_59_layer, 0,
  OPTIMIZED_CONV2D_TYPE,
  conv2d_nl_pool, forward_conv2d_nl_pool,
  &AI_NET_OBJ_INSTANCE, &conv2d_60_layer, AI_STATIC,
  .tensors = &conv2d_59_chain, 
  .groups = 1, 
  .nl_func = nl_func_relu_array_f32, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_2D_INIT(0, 0), 
  .filter_pad_r = AI_SHAPE_2D_INIT(0, 0), 
  .pool_size = AI_SHAPE_2D_INIT(2, 2), 
  .pool_stride = AI_SHAPE_2D_INIT(2, 2), 
  .pool_pad = AI_SHAPE_2D_INIT(0, 0), 
  .pool_func = pool_func_mp_array_f32, 
)

/* Layer #1: "conv2d_60" (Conv2D) */
  

/* Weight tensor #1 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_60_weights_array, AI_DATA_FORMAT_FLOAT, 
  NULL, NULL, 1152,
  AI_STATIC)

AI_TENSOR_OBJ_DECLARE(
  conv2d_60_weights,
  AI_SHAPE_INIT(16, 3, 3, 8),
  AI_STRIDE_INIT(288, 96, 32, 4),
  &conv2d_60_weights_array,
  AI_STATIC)

/* Weight tensor #2 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_60_bias_array, AI_DATA_FORMAT_FLOAT, 
  NULL, NULL, 16,
  AI_STATIC)

AI_TENSOR_OBJ_DECLARE(
  conv2d_60_bias,
  AI_SHAPE_INIT(1, 1, 16, 1),
  AI_STRIDE_INIT(64, 64, 4, 4),
  &conv2d_60_bias_array,
  AI_STATIC)


AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv2d_60_chain, AI_STATIC_CONST, 
  AI_TENSOR_LIST_ENTRY(&conv2d_59_output),
  AI_TENSOR_LIST_ENTRY(&conv2d_60_output),
  AI_TENSOR_LIST_ENTRY(&conv2d_60_weights, &conv2d_60_bias),
  AI_TENSOR_LIST_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv2d_60_layer, 3,
  OPTIMIZED_CONV2D_TYPE,
  conv2d_nl_pool, forward_conv2d_nl_pool,
  &AI_NET_OBJ_INSTANCE, &conv2d_61_layer, AI_STATIC,
  .tensors = &conv2d_60_chain, 
  .groups = 1, 
  .nl_func = nl_func_relu_array_f32, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_2D_INIT(0, 0), 
  .filter_pad_r = AI_SHAPE_2D_INIT(0, 0), 
  .pool_size = AI_SHAPE_2D_INIT(2, 2), 
  .pool_stride = AI_SHAPE_2D_INIT(2, 2), 
  .pool_pad = AI_SHAPE_2D_INIT(0, 0), 
  .pool_func = pool_func_mp_array_f32, 
)

/* Layer #2: "conv2d_61" (Conv2D) */
  

/* Weight tensor #1 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_61_weights_array, AI_DATA_FORMAT_FLOAT, 
  NULL, NULL, 4608,
  AI_STATIC)

AI_TENSOR_OBJ_DECLARE(
  conv2d_61_weights,
  AI_SHAPE_INIT(32, 3, 3, 16),
  AI_STRIDE_INIT(576, 192, 64, 4),
  &conv2d_61_weights_array,
  AI_STATIC)

/* Weight tensor #2 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_61_bias_array, AI_DATA_FORMAT_FLOAT, 
  NULL, NULL, 32,
  AI_STATIC)

AI_TENSOR_OBJ_DECLARE(
  conv2d_61_bias,
  AI_SHAPE_INIT(1, 1, 32, 1),
  AI_STRIDE_INIT(128, 128, 4, 4),
  &conv2d_61_bias_array,
  AI_STATIC)


AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv2d_61_chain, AI_STATIC_CONST, 
  AI_TENSOR_LIST_ENTRY(&conv2d_60_output),
  AI_TENSOR_LIST_ENTRY(&conv2d_61_output_in),
  AI_TENSOR_LIST_ENTRY(&conv2d_61_weights, &conv2d_61_bias),
  AI_TENSOR_LIST_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv2d_61_layer, 6,
  OPTIMIZED_CONV2D_TYPE,
  conv2d_nl_pool, forward_conv2d_nl_pool,
  &AI_NET_OBJ_INSTANCE, &dense_33_layer, AI_STATIC,
  .tensors = &conv2d_61_chain, 
  .groups = 1, 
  .nl_func = nl_func_relu_array_f32, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_2D_INIT(0, 0), 
  .filter_pad_r = AI_SHAPE_2D_INIT(0, 0), 
  .pool_size = AI_SHAPE_2D_INIT(2, 2), 
  .pool_stride = AI_SHAPE_2D_INIT(2, 2), 
  .pool_pad = AI_SHAPE_2D_INIT(0, 0), 
  .pool_func = pool_func_mp_array_f32, 
)

/* Layer #3: "dense_33" (Dense) */
  

/* Weight tensor #1 */
AI_ARRAY_OBJ_DECLARE(
  dense_33_weights_array, AI_DATA_FORMAT_LUT8_FLOAT, 
  NULL, NULL, 73728,
  AI_STATIC)

AI_TENSOR_OBJ_DECLARE(
  dense_33_weights,
  AI_SHAPE_INIT(1, 1, 128, 576),
  AI_STRIDE_INIT(73728, 73728, 576, 1),
  &dense_33_weights_array,
  AI_STATIC)

/* Weight tensor #2 */
AI_ARRAY_OBJ_DECLARE(
  dense_33_bias_array, AI_DATA_FORMAT_FLOAT, 
  NULL, NULL, 128,
  AI_STATIC)

AI_TENSOR_OBJ_DECLARE(
  dense_33_bias,
  AI_SHAPE_INIT(1, 1, 128, 1),
  AI_STRIDE_INIT(512, 512, 4, 4),
  &dense_33_bias_array,
  AI_STATIC)


AI_TENSOR_CHAIN_OBJ_DECLARE(
  dense_33_chain, AI_STATIC_CONST, 
  AI_TENSOR_LIST_ENTRY(&conv2d_61_output_out),
  AI_TENSOR_LIST_ENTRY(&dense_33_output),
  AI_TENSOR_LIST_ENTRY(&dense_33_weights, &dense_33_bias),
  AI_TENSOR_LIST_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  dense_33_layer, 10,
  DENSE_TYPE,
  dense, forward_dense,
  &AI_NET_OBJ_INSTANCE, &dense_33_nl_layer, AI_STATIC,
  .tensors = &dense_33_chain, 
)

/* Layer #4: "dense_33_nl" (Nonlinearity) */
  


AI_TENSOR_CHAIN_OBJ_DECLARE(
  dense_33_nl_chain, AI_STATIC_CONST, 
  AI_TENSOR_LIST_ENTRY(&dense_33_output),
  AI_TENSOR_LIST_ENTRY(&dense_33_nl_output),
  AI_TENSOR_LIST_EMPTY,
  AI_TENSOR_LIST_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  dense_33_nl_layer, 10,
  NL_TYPE,
  nl, forward_relu,
  &AI_NET_OBJ_INSTANCE, &dense_34_layer, AI_STATIC,
  .tensors = &dense_33_nl_chain, 
)

/* Layer #5: "dense_34" (Dense) */
  

/* Weight tensor #1 */
AI_ARRAY_OBJ_DECLARE(
  dense_34_weights_array, AI_DATA_FORMAT_FLOAT, 
  NULL, NULL, 1280,
  AI_STATIC)

AI_TENSOR_OBJ_DECLARE(
  dense_34_weights,
  AI_SHAPE_INIT(1, 1, 10, 128),
  AI_STRIDE_INIT(5120, 5120, 512, 4),
  &dense_34_weights_array,
  AI_STATIC)

/* Weight tensor #2 */
AI_ARRAY_OBJ_DECLARE(
  dense_34_bias_array, AI_DATA_FORMAT_FLOAT, 
  NULL, NULL, 10,
  AI_STATIC)

AI_TENSOR_OBJ_DECLARE(
  dense_34_bias,
  AI_SHAPE_INIT(1, 1, 10, 1),
  AI_STRIDE_INIT(40, 40, 4, 4),
  &dense_34_bias_array,
  AI_STATIC)


AI_TENSOR_CHAIN_OBJ_DECLARE(
  dense_34_chain, AI_STATIC_CONST, 
  AI_TENSOR_LIST_ENTRY(&dense_33_nl_output),
  AI_TENSOR_LIST_ENTRY(&dense_34_output),
  AI_TENSOR_LIST_ENTRY(&dense_34_weights, &dense_34_bias),
  AI_TENSOR_LIST_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  dense_34_layer, 12,
  DENSE_TYPE,
  dense, forward_dense,
  &AI_NET_OBJ_INSTANCE, &dense_34_nl_layer, AI_STATIC,
  .tensors = &dense_34_chain, 
)

/* Layer #6: "dense_34_nl" (Nonlinearity) */
  


AI_TENSOR_CHAIN_OBJ_DECLARE(
  dense_34_nl_chain, AI_STATIC_CONST, 
  AI_TENSOR_LIST_ENTRY(&dense_34_output),
  AI_TENSOR_LIST_ENTRY(&dense_34_nl_output),
  AI_TENSOR_LIST_EMPTY,
  AI_TENSOR_LIST_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  dense_34_nl_layer, 12,
  SM_TYPE,
  sm, forward_sm,
  &AI_NET_OBJ_INSTANCE, &dense_34_nl_layer, AI_STATIC,
  .tensors = &dense_34_nl_chain, 
)


AI_NETWORK_OBJ_DECLARE(
  AI_NET_OBJ_INSTANCE,
  AI_BUFFER_OBJ_INIT(AI_BUFFER_FORMAT_U8,
                     1, 1, 103976, 1,
                     NULL),
  AI_BUFFER_OBJ_INIT(AI_BUFFER_FORMAT_U8,
                     1, 1, 26116, 1,
                     NULL),
  &input_0_output, &dense_34_nl_output,
  &conv2d_59_layer, 0)


AI_DECLARE_STATIC
ai_bool network_configure_activations(
  ai_network* net_ctx, const ai_buffer* activation_buffer)
{
  AI_ASSERT(net_ctx &&  activation_buffer && activation_buffer->data)

  ai_ptr activations = AI_PTR(AI_PTR_ALIGN(activation_buffer->data, 4));
  AI_ASSERT( activations )
  AI_FLAG_SET(net_ctx->flags, AI_NETWORK_FLAG_OUT_COPY);

  {
    /* Updating activations (byte) offsets */
    input_0_output_array.data = NULL;
  input_0_output_array.data_start = NULL;
  conv2d_59_output_array.data = activations + 0;
  conv2d_59_output_array.data_start = activations + 0;
  conv2d_60_output_array.data = activations + 18880;
  conv2d_60_output_array.data_start = activations + 18880;
  conv2d_61_output_in_array.data = activations + 0;
  conv2d_61_output_in_array.data_start = activations + 0;
  conv2d_61_output_out_array.data = activations + 0;
  conv2d_61_output_out_array.data_start = activations + 0;
  dense_33_output_array.data = activations + 4608;
  dense_33_output_array.data_start = activations + 4608;
  dense_33_nl_output_array.data = activations + 4608;
  dense_33_nl_output_array.data_start = activations + 4608;
  dense_34_output_array.data = activations + 0;
  dense_34_output_array.data_start = activations + 0;
  dense_34_nl_output_array.data = activations + 0;
  dense_34_nl_output_array.data_start = activations + 0;
  
  }
  return true;
}

AI_DECLARE_STATIC
ai_bool network_configure_weights(
  ai_network* net_ctx, const ai_buffer* weights_buffer)
{
  AI_ASSERT(net_ctx &&  weights_buffer && weights_buffer->data)

  ai_ptr weights = AI_PTR(weights_buffer->data);
  AI_ASSERT( weights )

  {
    /* Updating weights (byte) offsets */
    conv2d_59_weights_array.format |= AI_FMT_FLAG_CONST;
  conv2d_59_weights_array.data = weights + 0;
  conv2d_59_weights_array.data_start = weights + 0;
  conv2d_59_bias_array.format |= AI_FMT_FLAG_CONST;
  conv2d_59_bias_array.data = weights + 288;
  conv2d_59_bias_array.data_start = weights + 288;
  conv2d_60_weights_array.format |= AI_FMT_FLAG_CONST;
  conv2d_60_weights_array.data = weights + 320;
  conv2d_60_weights_array.data_start = weights + 320;
  conv2d_60_bias_array.format |= AI_FMT_FLAG_CONST;
  conv2d_60_bias_array.data = weights + 4928;
  conv2d_60_bias_array.data_start = weights + 4928;
  conv2d_61_weights_array.format |= AI_FMT_FLAG_CONST;
  conv2d_61_weights_array.data = weights + 4992;
  conv2d_61_weights_array.data_start = weights + 4992;
  conv2d_61_bias_array.format |= AI_FMT_FLAG_CONST;
  conv2d_61_bias_array.data = weights + 23424;
  conv2d_61_bias_array.data_start = weights + 23424;
  dense_33_weights_array.format |= AI_FMT_FLAG_CONST;
  dense_33_weights_array.data = weights + 23552;
  dense_33_weights_array.data_start = weights + 97280;
  dense_33_bias_array.format |= AI_FMT_FLAG_CONST;
  dense_33_bias_array.data = weights + 98304;
  dense_33_bias_array.data_start = weights + 98304;
  dense_34_weights_array.format |= AI_FMT_FLAG_CONST;
  dense_34_weights_array.data = weights + 98816;
  dense_34_weights_array.data_start = weights + 98816;
  dense_34_bias_array.format |= AI_FMT_FLAG_CONST;
  dense_34_bias_array.data = weights + 103936;
  dense_34_bias_array.data_start = weights + 103936;
  
  }

  return true;
}

/**  PUBLIC APIs SECTION  *****************************************************/

AI_API_ENTRY
ai_bool ai_network_get_info(
  ai_handle network, ai_network_report* report)
{
  ai_network* net_ctx = AI_NETWORK_ACQUIRE_CTX(network);

  if ( report && net_ctx )
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
      .tool_api_version  = {AI_TOOLS_API_VERSION_MAJOR, AI_TOOLS_API_VERSION_MINOR,
                            AI_TOOLS_API_VERSION_MICRO, 0x0},

      .api_version            = ai_platform_api_get_version(),
      .interface_api_version  = ai_platform_interface_api_get_version(),
      
      .n_macc            = 1202046,
      .n_inputs          = AI_NETWORK_IN_NUM,
      .inputs            = AI_BUFFER_OBJ_INIT(
                              AI_BUFFER_FORMAT_FLOAT,
                              64,
                              40,
                              1,
                              1, NULL),
      .n_outputs         = AI_NETWORK_OUT_NUM,
      .outputs           = AI_BUFFER_OBJ_INIT(
                              AI_BUFFER_FORMAT_FLOAT,
                              1,
                              1,
                              10,
                              1, NULL),
      .activations       = net_ctx->activations,
      .weights           = net_ctx->params,
      .n_nodes           = 0,
      .signature         = net_ctx->signature,
    };

    AI_FOR_EACH_NODE_DO(node, net_ctx->input_node)
    {
      r.n_nodes++;
    }

    *report = r;

    return ( r.n_nodes>0 ) ? true : false;
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
    &AI_NET_OBJ_INSTANCE,
    AI_TOOLS_API_VERSION_MAJOR, AI_TOOLS_API_VERSION_MINOR, AI_TOOLS_API_VERSION_MICRO);
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
  ai_network* net_ctx = ai_platform_network_init(network, params);
  if ( !net_ctx ) return false;

  ai_bool ok = true;
  ok &= network_configure_weights(net_ctx, &params->params);
  ok &= network_configure_activations(net_ctx, &params->activations);
  
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
#undef AI_TOOLS_VERSION_MAJOR
#undef AI_TOOLS_VERSION_MINOR
#undef AI_TOOLS_VERSION_MICRO
#undef AI_TOOLS_API_VERSION_MAJOR
#undef AI_TOOLS_API_VERSION_MINOR
#undef AI_TOOLS_API_VERSION_MICRO
#undef AI_TOOLS_DATE_TIME
#undef AI_TOOLS_COMPILE_TIME

