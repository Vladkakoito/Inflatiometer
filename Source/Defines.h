#pragma once

#define PRAGMA_MESSAGE(x) _Pragma(#x)
#define TODO(msg) PRAGMA_MESSAGE(message ("TODO: " #msg))
