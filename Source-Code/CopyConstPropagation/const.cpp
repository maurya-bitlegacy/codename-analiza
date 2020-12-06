 #include "const.h"

 std::string CONST_toString(PossibleValueKinds s)
	{

	  if(s == PossibleValueKinds::NAC) {
	    return "NAC";
	  } else if(s == PossibleValueKinds::UNDEFINED) {
	    return "UNDEFINED";
	  } else if(s == PossibleValueKinds::CONSTANT) {
	    return "CONSTANT";
	  }else {
	    return "NAC";
	  }
	}