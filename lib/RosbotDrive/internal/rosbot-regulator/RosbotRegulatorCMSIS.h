#ifndef __ROSBOT_REGULATOR_CMSIS_H__
#define __ROSBOT_REGULATOR_CMSIS_H__

#include <mbed.h>
#include "RosbotRegulator.h"
#include <arm_math.h>

/***************************CMSIS-DSP-PID***************************/
/*
 * Copyright (C) 2010-2017 ARM Limited or its affiliates. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#if 0
/**
* @brief  Process function for the floating-point PID Control.
* @param[in,out] S   is an instance of the floating-point PID Control structure
* @param[in]     in  input sample to process
* @return out processed output sample.
*/
static __INLINE float32_t arm_pid_f32(
    arm_pid_instance_f32 *S,
    float32_t in)
{
    float32_t out;

    /* y[n] = y[n-1] + A0 * x[n] + A1 * x[n-1] + A2 * x[n-2]  */
    out = (S->A0 * in) +
          (S->A1 * S->state[0]) + (S->A2 * S->state[1]) + (S->state[2]);

    /* Update state */
    S->state[1] = S->state[0];
    S->state[0] = in;
    S->state[2] = out;

    /* return to application */
    return (out);
}
#endif

void arm_pid_init_f32(
  arm_pid_instance_f32 * S,
  int32_t resetStateFlag)
{

  /* Derived coefficient A0 */
  S->A0 = S->Kp + S->Ki + S->Kd;

  /* Derived coefficient A1 */
  S->A1 = (-S->Kp) - ((float32_t) 2.0 * S->Kd);

  /* Derived coefficient A2 */
  S->A2 = S->Kd;

  /* Check whether state needs reset or not */
  if(resetStateFlag)
  {
    /* Clear the state buffer.  The size will be always 3 samples */
    memset(S->state, 0, 3u * sizeof(float32_t));
  }

}

/**    
* @brief  Reset function for the floating-point PID Control.   
* @param[in] *S	Instance pointer of PID control data structure.   
* @return none.    
* \par Description:   
* The function resets the state buffer to zeros.    
*/
void arm_pid_reset_f32(
  arm_pid_instance_f32 * S)
{

  /* Clear the state buffer.  The size will be always 3 samples */
  memset(S->state, 0, 3u * sizeof(float32_t));
}
/***************************CMSIS-DSP-PID***************************/


class RosbotRegulatorCMSIS : public RosbotRegulator
{
public:
    RosbotRegulatorCMSIS(const RosbotRegulator_params &params)
    :RosbotRegulator(params),
    {
        _state.Kp = _params.kp;
        _state.Ki = _params.ki;
        _state.Kd = _params.kd;
        arm_pid_init_f32(&_state,1);
    }

    ~RosbotRegulatorCMSIS(){}

    void updateParams(const RosbotRegulator_params &params)
    {
        _params = params;
        _state.Kp = _params.kp;
        _state.Ki = _params.ki;
        _state.Kd = _params.kd;
        arm_pid_init_f32(&_state,1);
    }

    void getParams(RosbotRegulator_params &params)
    {
        params = _params;
    }

    float updateState(float setpoint, float feedback)
    {
        
    }

    float getPidout()
    {
        return _pidout;
    }

    float getError()
    {
        return _error;
    }

    void reset()
    {
        _state.Kp = _params.kp;
        _state.Ki = _params.ki;
        _state.Kd = _params.kd;
        arm_pid_init_f32(&_state,1);
    }

private:
    arm_pid_instance_f32 _state;
    float _error;
    float _pidout;
};

#endif /* __ROSBOT_REGULATOR_CMSIS_H__ */