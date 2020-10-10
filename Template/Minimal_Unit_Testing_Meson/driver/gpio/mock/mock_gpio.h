#ifndef GPIO_MOCK_MOCK_GPIO_H_
#define GPIO_MOCK_MOCK_GPIO_H_

#include "fff.h"

#include <gpio/gpio.h>

FAKE_VOID_FUNC(gpio__init, GPIO_s *, GPIO_TypeDef *, uint32_t);

FAKE_VOID_FUNC(gpio__set, const GPIO_s *);
FAKE_VOID_FUNC(gpio__reset, const GPIO_s *);

FAKE_VALUE_FUNC(bool, gpio__get, const GPIO_s *);

#endif
