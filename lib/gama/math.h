#pragma once

#ifndef GM_MATH
#define GM_MATH
#endif

/**
 * @def PI
 * @brief The mathematical constant Pi (π).
 */
#define PI 3.14159265358979324
/**
 * @def M_PI
 * @brief Alias for the mathematical constant Pi (π).
 */
#define M_PI PI
/**
 * @def EPS
 * @brief A small epsilon value used for floating-point comparisons.
 */
#define EPS 1e-17
/**
 * @def ln10
 * @brief The natural logarithm of 10.
 */
#define ln10 2.30258509299404590109
/**
 * @def INF
 * @brief Represents positive infinity for double-precision floating-point numbers.
 */
#define INF 1.0 / 0.0
/**
 * @def NAN
 * @brief Represents "Not a Number" for double-precision floating-point numbers.
 */
#define NAN 0.0 / 0.0
/**
 * @def MAX_double
 * @brief The maximum representable value for a double-precision floating-point number.
 */
#define MAX_double 1.7976931348623158e308

/**
 * @internal
 * @brief Structure to represent a double in scientific notation components.
 */
struct special {
  int sign;
  double mantisa;
  double pow;
  int e;
};

// Function declarations (forward declarations for custom implementations)
double tan(double x);
double cos(double x);
double sin(double x);
double ceil(double x);
double floor(double x);
double atan(double x);
double acos(double x);
double asin(double x);
double fmod(double x, double y);
double exp(double x);
double log(double x);
double sqrt(double x);
double pow(double base, double exp);
double fabs(double x);

/**
 * @internal
 * @brief Translates a double-precision float into its scientific notation components.
 * @param x The double to translate.
 * @param _special Pointer to a `special` struct to store the components.
 */
void translate(double x, struct special *_special);
/**
 * @internal
 * @brief Helper for atan, specifically for values between -1 and 1.
 */
double atan_1_1(double x);
/**
 * @internal
 * @brief Helper function to normalize an angle to the range [-PI, PI].
 * @param x The angle in radians.
 * @return The normalized angle.
 */
double delete (double x); // Note: 'delete' is a C++ keyword, consider renaming.
/**
 * @internal
 * @brief Helper for log, for values outside [0, 2).
 */
double log_other(double x);
/**
 * @internal
 * @brief Helper for log, for values between [0, 2).
 */
double log_0_2(double x);

/**
 * @brief Calculates the cosine of an angle (in radians).
 * @param x The angle in radians.
 * @return The cosine of x.
 */
double cos(double x) {
  x = delete (x);
  return sin(PI / 2 - x);
}

/**
 * @internal
 * @brief Helper function to normalize an angle to the range [-PI, PI].
 * @param x The angle in radians.
 * @return The normalized angle.
 */
double delete (double x) {
  while (x > PI || x < -PI) {
    x += x > PI ? -2 * PI : 2 * PI;
  }
  return x;
}

/**
 * @brief Calculates the sine of an angle (in radians).
 * @param x The angle in radians.
 * @return The sine of x.
 */
double sin(double x) {
  x = delete (x);
  double result = x, temp = x;
  double i = 1.;
  while (fabs(result) > EPS) {
    result = -1 * result * x * x / (2 * i * (2 * i + 1));
    i += 1.;
    temp += result;
  }
  return temp;
}

/**
 * @brief Calculates the tangent of an angle (in radians).
 * @param x The angle in radians.
 * @return The tangent of x, or NAN if x is an odd multiple of PI/2.
 */
double tan(double x) {
  struct special _special;
  double temp, c = cos(x);
  translate(c, &_special);
  ;
  temp = _special.sign * sin(x) / _special.mantisa;
  temp /= _special.pow;
  return (x != -PI / 2 && x != PI / 2) ? temp : NAN;
}

/**
 * @brief Calculates the smallest integer value greater than or equal to x.
 * @param x The floating-point value.
 * @return The ceiling of x.
 */
double ceil(double x) {
  int i;
  double temp = x < 0 ? -x : x;
  for (i = 0; i < temp; i++) {
  }
  return x < 0 ? -i + 1 : i;
}

/**
 * @brief Calculates the largest integer value less than or equal to x.
 * @param x The floating-point value.
 * @return The floor of x.
 */
double floor(double x) {
  int i;
  double temp = x < 0 ? -x : x;
  for (i = 0; i < temp; i++) {
  }
  return x < 0 ? -i : x == 0 ? 0 : i - 1;
}

/**
 * @brief Calculates the arctangent of x.
 * @param x The floating-point value.
 * @return The arctangent of x in radians, in the range [-PI/2, PI/2].
 */
double atan(double x) {
  double temp = 0;
  temp = (x < 1 && x > -1) ? atan_1_1(x) : temp;
  temp = x == 1 ? PI / 4 : x == -1 ? -PI / 4 : x == 0 ? 0 : temp;
  temp = x > 1    ? PI / 2 - atan_1_1(1 / x)
         : x < -1 ? -PI / 2 - atan_1_1(1 / x)
                  : temp;
  return temp;
}

/**
 * @internal
 * @brief Helper for atan, specifically for values between -1 and 1.
 * @param x The value.
 * @return The arctangent of x.
 */
double atan_1_1(double x) {
  double result = x, temp = x, i = 1;
  while (fabs(result) > EPS) {
    result = -1 * result * x * x * (2 * i - 1) / (2 * i + 1);
    i += 1;
    temp += result;
  }
  return temp;
}

/**
 * @brief Calculates the arccosine of x.
 * @param x The floating-point value, expected to be in the range [-1, 1].
 * @return The arccosine of x in radians, in the range [0, PI], or NAN if x is out of range.
 */
double acos(double x) {
  return (x <= 1 && x >= -1)
             ? fabs(x) == 1 ? PI * (1 - x) / 2 : PI / 2. - asin(x)
             : NAN;
}

/**
 * @brief Calculates the arcsine of x.
 * @param x The floating-point value, expected to be in the range [-1, 1].
 * @return The arcsine of x in radians, in the range [-PI/2, PI/2], or NAN if x is out of range.
 */
double asin(double x) {
  double result = x, temp = x;
  double i = 1;
  while (fabs(result) > EPS) {
    if (x < -1 || x > 1) {
      temp = NAN;
      break;
    }
    if (x == 1 || x == -1) {
      temp = PI / 2 * x;
      break;
    }
    result *=
        x * x * (2 * i - 1) * (2 * i) * (2 * i - 1) / ((2 * i + 1) * 4 * i * i);
    i += 1;
    temp += result;
  }
  return temp;
}

/**
 * @brief Calculates the floating-point remainder of x/y.
 * @param x The numerator.
 * @param y The denominator.
 * @return The remainder with the same sign as x.
 */
double fmod(double x, double y) {
  double result = fabs(x);
  y = fabs(y);
  int i = 1;
  while (result > y) {
    result = fabs(x) - y * i;
    i++;
  }
  return x < 0 ? -result : result;
}

/**
 * @brief Calculates the base-e exponential of x (e^x).
 * @param x The exponent.
 * @return The value of e raised to the power of x.
 */
double exp(double x) {
  double result = 1, temp = 1;
  double i = 1;
  int flag = 0;
  if (x < 0) {
    x *= -1;
    flag = 1;
  }
  while (fabs(result) > EPS) {
    result *= x / i;
    i += 1;
    temp += result;
    if (temp > MAX_double) {
      temp = INF;
      break;
    }
  }
  temp = flag == 1 ? temp > MAX_double ? 0 : 1. / temp : temp;
  return temp = temp > MAX_double ? INF : temp;
}

/**
 * @brief Calculates the natural logarithm of x (ln(x)).
 * @param x The floating-point value.
 * @return The natural logarithm of x, or NAN if x is negative, or -INF if x is zero.
 */
double log(double x) {
  double temp;
  return temp = (x > 0 && x < 2) ? log_0_2(x) : log_other(x);
}

/**
 * @internal
 * @brief Helper for log, for values between [0, 2).
 * @param x The value.
 * @return The natural logarithm of x.
 */
double log_0_2(double x) {
  x--;
  double result = x, temp = x;
  double i = 2;
  while (fabs(result) > EPS) {
    result *= -x * (i - 1) / i;
    i += 1;
    temp += result;
  }
  return temp;
}

/**
 * @internal
 * @brief Helper for log, for values outside [0, 2).
 * @param x The value.
 * @return The natural logarithm of x.
 */
double log_other(double x) {
  struct special _special;
  translate(x, &_special);
  x = _special.mantisa * _special.sign / 10;
  double result;
  result = x < 0 ? -NAN : x == 0 ? -INF : log_0_2(x) + (_special.e + 1) * ln10;
  return result;
}

/**
 * @brief Calculates the square root of x.
 * @param x The non-negative floating-point value.
 * @return The square root of x, or NAN if x is negative.
 */
double sqrt(double x) {
  double result = 4, temp = 0;
  while (fabs(result - temp) > EPS) {
    if (x < 0) {
      result = -NAN;
      break;
    }
    temp = result;
    result = (temp + x / temp) / 2;
  }
  return result;
}

/**
 * @brief Calculates the base raised to the power of the exponent (base^exp).
 * @param base The base value.
 * @param vexp The exponent value.
 * @return The result of base raised to the power of exp.
 */
double pow(double base, double vexp) {
  double result;
  result = exp(vexp * log(base));
  return result;
}

/**
 * @brief Calculates the absolute value of a double.
 * @param x The floating-point value.
 * @return The absolute value of x.
 */
double fabs(double x) { return x < 0 ? x *= -1. : x; }

/**
 * @internal
 * @brief Translates a double-precision float into its scientific notation components.
 * @param x The double to translate.
 * @param _special Pointer to a `special` struct to store the components.
 */
void translate(double x, struct special *_special) {
  double i = 1;
  int es = 0;
  _special->sign = x < 0 ? -1 : 1;
  x *= _special->sign;
  if (x >= 10) {
    while (x >= 10) {
      x /= 10.;
      i *= 10;
      es++;
    }
  } else if (x < 1 && x > 0) {
    while (x < 1) {
      x *= 10;
      i /= 10;
      es--;
    }
  }
  _special->mantisa = x;
  _special->pow = i;
  _special->e = es;
}

/**
 * @brief Returns the smaller of two integer values.
 * @param a The first integer.
 * @param b The second integer.
 * @return The smaller of a and b.
 */
int min(int a, int b) { return a > b ? b : a; }
/**
 * @brief Returns the smaller of two double values.
 * @param a The first double.
 * @param b The second double.
 * @return The smaller of a and b.
 */
double fmin(double a, double b) { return a > b ? b : a; }

/**
 * @brief Returns the larger of two integer values.
 * @param a The first integer.
 * @param b The second integer.
 * @return The larger of a and b.
 */
int max(int a, int b) { return a < b ? b : a; }
/**
 * @brief Returns the larger of two double values.
 * @param a The first double.
 * @param b The second double.
 * @return The larger of a and b.
 */
double fmax(double a, double b) { return a < b ? b : a; }
