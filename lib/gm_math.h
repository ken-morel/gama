#pragma once

#define PI 3.14159265358979324
#define M_PI PI
#define EPS 1e-17
#define ln10 2.30258509299404590109
#define INF 1.0 / 0.0
#define NAN 0.0 / 0.0
#define MAX_double 1.7976931348623158e308

struct special {
  int sign;
  double mantisa;
  double pow;
  int e;
};

double tan(double x);
double cos(double x);
double sin(double x);
int abs(int x);
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

void translate(double x, struct special *_special);
double atan_1_1(double x);
double delete(double x);
double log_other(double x);
double log_0_2(double x);

double cos(double x) {
  x = delete (x);
  return sin(PI / 2 - x);
}

double delete(double x) {
  while (x > PI || x < -PI) {
    x += x > PI ? -2 * PI : 2 * PI;
  }
  return x;
}

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

double tan(double x) {
  struct special _special;
  double temp, c = cos(x);
  translate(c, &_special);
  ;
  temp = _special.sign * sin(x) / _special.mantisa;
  temp /= _special.pow;
  return (x != -PI / 2 && x != PI / 2) ? temp : NAN;
}

int abs(int x) { return x < 0 ? x *= -1 : x; }

double ceil(double x) {
  int i;
  double temp = x < 0 ? -x : x;
  for (i = 0; i < temp; i++) {
  }
  return x < 0 ? -i + 1 : i;
}

double floor(double x) {
  int i;
  double temp = x < 0 ? -x : x;
  for (i = 0; i < temp; i++) {
  }
  return x < 0 ? -i : x == 0 ? 0 : i - 1;
}

double atan(double x) {
  double temp = 0;
  temp = (x < 1 && x > -1) ? atan_1_1(x) : temp;
  temp = x == 1 ? PI / 4 : x == -1 ? -PI / 4 : x == 0 ? 0 : temp;
  temp = x > 1    ? PI / 2 - atan_1_1(1 / x)
         : x < -1 ? -PI / 2 - atan_1_1(1 / x)
                  : temp;
  return temp;
}

double atan_1_1(double x) {
  double result = x, temp = x, i = 1;
  while (fabs(result) > EPS) {
    result = -1 * result * x * x * (2 * i - 1) / (2 * i + 1);
    i += 1;
    temp += result;
  }
  return temp;
}

double acos(double x) {
  return (x <= 1 && x >= -1)
             ? fabs(x) == 1 ? PI * (1 - x) / 2 : PI / 2. - asin(x)
             : NAN;
}

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

double log(double x) {
  double temp;
  return temp = (x > 0 && x < 2) ? log_0_2(x) : log_other(x);
}

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

double log_other(double x) {
  struct special _special;
  translate(x, &_special);
  x = _special.mantisa * _special.sign / 10;
  double result;
  result = x < 0 ? -NAN : x == 0 ? -INF : log_0_2(x) + (_special.e + 1) * ln10;
  return result;
}

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

double pow(double base, double vexp) {
  double result;
  result = exp(vexp * log(base));
  return result;
}

double fabs(double x) { return x < 0 ? x *= -1. : x; }

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

int min(int a, int b) { return a > b ? b : a; }
double fmin(double a, double b) { return a > b ? b : a; }

int max(int a, int b) { return a < b ? b : a; }
double fmax(double a, double b) { return a < b ? b : a; }
