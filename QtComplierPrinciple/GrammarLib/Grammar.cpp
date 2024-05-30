#include "Grammar.h"


Production::Production() {
	left = "";
	right.clear();
	have_null = 0;
}