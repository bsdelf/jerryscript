/* Copyright 2014 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "ecma-comparison.h"
#include "ecma-conversion.h"
#include "ecma-globals.h"
#include "ecma-try-catch-macro.h"
#include "globals.h"

/** \addtogroup ecma ECMA
 * @{
 *
 * \addtogroup ecmacomparison ECMA comparison
 * @{
 */

/**
 * ECMA abstract equality comparison routine.
 *
 * See also: ECMA-262 v5, 11.9.3
 *
 * @return true - if values are equal,
 *         false - otherwise.
 */
ecma_completion_value_t
ecma_op_abstract_equality_compare (ecma_value_t x, /**< first operand */
                                   ecma_value_t y) /**< second operand */
{
  ecma_completion_value_t ret_value;

  const bool is_x_undefined = ecma_is_value_undefined (x);
  const bool is_x_null = ecma_is_value_null (x);
  const bool is_x_boolean = ecma_is_value_boolean (x);
  const bool is_x_number = (x.value_type == ECMA_TYPE_NUMBER);
  const bool is_x_string = (x.value_type == ECMA_TYPE_STRING);
  const bool is_x_object = (x.value_type == ECMA_TYPE_OBJECT);

  const bool is_y_undefined = ecma_is_value_undefined (y);
  const bool is_y_null = ecma_is_value_null (y);
  const bool is_y_boolean = ecma_is_value_boolean (y);
  const bool is_y_number = (y.value_type == ECMA_TYPE_NUMBER);
  const bool is_y_string = (y.value_type == ECMA_TYPE_STRING);
  const bool is_y_object = (y.value_type == ECMA_TYPE_OBJECT);

  const bool is_types_equal = ((is_x_undefined && is_y_undefined)
                               || (is_x_null && is_y_null)
                               || (is_x_boolean && is_y_boolean)
                               || (is_x_number && is_y_number)
                               || (is_x_string && is_y_string)
                               || (is_x_object && is_y_object));

  if (is_types_equal)
  {
    // 1.

    if (is_x_undefined
        || is_x_null)
    {
      // a., b.
      ret_value = ecma_make_simple_completion_value (ECMA_SIMPLE_VALUE_TRUE);
    }
    else if (is_x_number)
    { // c.
      ecma_number_t x_num = *(ecma_number_t*)(ECMA_GET_POINTER(x.value));
      ecma_number_t y_num = *(ecma_number_t*)(ECMA_GET_POINTER(y.value));

      TODO(Implement according to ECMA);

      bool is_equal = (x_num == y_num);

      ret_value = ecma_make_simple_completion_value (is_equal ? ECMA_SIMPLE_VALUE_TRUE : ECMA_SIMPLE_VALUE_FALSE);
    }
    else if (is_x_string)
    { // d.
      ecma_string_t* x_str_p = ECMA_GET_POINTER(x.value);
      ecma_string_t* y_str_p = ECMA_GET_POINTER(y.value);

      bool is_equal = ecma_compare_ecma_string_to_ecma_string (x_str_p, y_str_p);

      ret_value = ecma_make_simple_completion_value (is_equal ? ECMA_SIMPLE_VALUE_TRUE : ECMA_SIMPLE_VALUE_FALSE);
    }
    else if (is_x_boolean)
    { // e.
      bool is_equal = (x.value == y.value);

      ret_value = ecma_make_simple_completion_value (is_equal ? ECMA_SIMPLE_VALUE_TRUE : ECMA_SIMPLE_VALUE_FALSE);
    }
    else
    { // f.
      JERRY_ASSERT(is_x_object);

      bool is_equal = (x.value == y.value);

      ret_value = ecma_make_simple_completion_value (is_equal ? ECMA_SIMPLE_VALUE_TRUE : ECMA_SIMPLE_VALUE_FALSE);
    }
  }
  else if ((is_x_null && is_y_undefined)
           || (is_x_undefined && is_y_null))
  { // 2., 3.
    ret_value = ecma_make_simple_completion_value (ECMA_SIMPLE_VALUE_TRUE);
  }
  else
  {
    JERRY_UNIMPLEMENTED();
  }

  return ret_value;
} /* ecma_op_abstract_equality_compare */

/**
 * ECMA strict equality comparison routine.
 *
 * See also: ECMA-262 v5, 11.9.6
 *
 * @return true - if values are strict equal,
 *         false - otherwise.
 */
bool
ecma_op_strict_equality_compare (ecma_value_t x, /**< first operand */
                                 ecma_value_t y) /**< second operand */
{
  const bool is_x_undefined = ecma_is_value_undefined (x);
  const bool is_x_null = ecma_is_value_null (x);
  const bool is_x_boolean = ecma_is_value_boolean (x);
  const bool is_x_number = (x.value_type == ECMA_TYPE_NUMBER);
  const bool is_x_string = (x.value_type == ECMA_TYPE_STRING);
  const bool is_x_object = (x.value_type == ECMA_TYPE_OBJECT);

  const bool is_y_undefined = ecma_is_value_undefined (y);
  const bool is_y_null = ecma_is_value_null (y);
  const bool is_y_boolean = ecma_is_value_boolean (y);
  const bool is_y_number = (y.value_type == ECMA_TYPE_NUMBER);
  const bool is_y_string = (y.value_type == ECMA_TYPE_STRING);
  const bool is_y_object = (y.value_type == ECMA_TYPE_OBJECT);

  const bool is_types_equal = ((is_x_undefined && is_y_undefined)
                               || (is_x_null && is_y_null)
                               || (is_x_boolean && is_y_boolean)
                               || (is_x_number && is_y_number)
                               || (is_x_string && is_y_string)
                               || (is_x_object && is_y_object));

  // 1. If Type (x) is different from Type (y), return false.
  if (!is_types_equal)
  {
    return false;
  }

  // 2. If Type (x) is Undefined, return true.
  if (is_x_undefined)
  {
    return true;
  }

  // 3. If Type (x) is Null, return true.
  if (is_x_null)
  {
    return true;
  }

  // 4. If Type (x) is Number, then
  if (is_x_number)
  {
    //a. If x is NaN, return false.
    //b. If y is NaN, return false.
    //c. If x is the same Number value as y, return true.
    //d. If x is +0 and y is -0, return true.
    //e. If x is -0 and y is +0, return true.

    ecma_number_t x_num = *(ecma_number_t*) (ECMA_GET_POINTER (x.value));
    ecma_number_t y_num = *(ecma_number_t*) (ECMA_GET_POINTER (y.value));

    TODO (Implement according to ECMA);

    return (x_num == y_num);
  }

  // 5. If Type (x) is String, then return true if x and y are exactly the same sequence of characters
  // (same length and same characters in corresponding positions); otherwise, return false.
  if (is_x_string)
  {
    ecma_string_t* x_str_p = ECMA_GET_POINTER (x.value);
    ecma_string_t* y_str_p = ECMA_GET_POINTER (y.value);

    return ecma_compare_ecma_string_to_ecma_string (x_str_p, y_str_p);
  }

  // 6. If Type (x) is Boolean, return true if x and y are both true or both false; otherwise, return false.
  if (is_x_boolean)
  {
    return (x.value == y.value);
  }

  // 7. Return true if x and y refer to the same object. Otherwise, return false.
  JERRY_ASSERT (is_x_object);

  return (ECMA_GET_POINTER (x.value) == ECMA_GET_POINTER (y.value));
} /* ecma_op_strict_equality_compare */

/**
 * ECMA abstract relational comparison routine.
 *
 * See also: ECMA-262 v5, 11.8.5
 *
 * @return completion value
 *         Returned value must be freed with ecma_free_completion_value
 */
ecma_completion_value_t
ecma_op_abstract_relational_compare (ecma_value_t x, /**< first operand */
                                     ecma_value_t y, /**< second operand */
                                     bool left_first) /**< 'LeftFirst' flag */
{
  ecma_completion_value_t ret_value, px, py;

  ecma_value_t first_converted_value = left_first ? x : y;
  ecma_value_t second_converted_value = left_first ? y : x;

  // 1., 2.
  ECMA_TRY_CATCH(prim_first_converted_value,
                 ecma_op_to_primitive (first_converted_value, ECMA_PREFERRED_TYPE_NUMBER),
                 ret_value);
  ECMA_TRY_CATCH(prim_second_converted_value,
                 ecma_op_to_primitive (second_converted_value, ECMA_PREFERRED_TYPE_NUMBER),
                 ret_value);

  px = left_first ? prim_first_converted_value : prim_second_converted_value;
  py = left_first ? prim_second_converted_value : prim_first_converted_value;

  const bool is_px_string = (px.u.value.value_type == ECMA_TYPE_STRING);
  const bool is_py_string = (py.u.value.value_type == ECMA_TYPE_STRING);

  if (!(is_px_string && is_py_string))
  { // 3.
    // a.
    ECMA_TRY_CATCH(nx, ecma_op_to_number (px.u.value), ret_value);

    // b.
    ECMA_TRY_CATCH(ny, ecma_op_to_number (py.u.value), ret_value);

    ecma_number_t* num_x_p = (ecma_number_t*)ECMA_GET_POINTER(nx.u.value.value);
    ecma_number_t* num_y_p = (ecma_number_t*)ECMA_GET_POINTER(ny.u.value.value);

    TODO(/* Implement according to ECMA */);

    if (*num_x_p >= *num_y_p)
    {
      ret_value = ecma_make_simple_completion_value (ECMA_SIMPLE_VALUE_FALSE);
    }
    else
    {
      ret_value = ecma_make_simple_completion_value (ECMA_SIMPLE_VALUE_TRUE);
    }

    ECMA_FINALIZE(ny);
    ECMA_FINALIZE(nx);
  }
  else
  { // 4.
    JERRY_UNIMPLEMENTED();
  }

  ECMA_FINALIZE(prim_second_converted_value);
  ECMA_FINALIZE(prim_first_converted_value);

  return ret_value;
} /* ecma_op_abstract_relational_compare */

/**
 * @}
 * @}
 */
