	// {
	//     if (auto* constant = llvm::dyn_cast<llvm::Constant>(v))
	//       return Constantv{constant};
 //   	 return state[v];
	// }

  // 	Constantv ConstantAnalysis:: evaluateBinaryOperator(llvm::BinaryOperator& binOp, ConstantState& state)
  // 	{
	 //    auto* op1   = binOp.getOperand(0);
	 //    auto* op2   = binOp.getOperand(1);
	 //    auto value1 = getConstantValueFor(op1, state);
	 //    auto value2 = getConstantValueFor(op2, state);

  //   	if (value1.isConstant() && value2.isConstant())
  //   	{
  //     		auto& layout = binOp.getModule()->getDataLayout();
  //    	 	auto eval    = ConstantFoldBinaryOpOperands(binOp.getOpcode(), value1.value, value2.value, layout);
  //    		return llvm::isa<llvm::ConstantExpr>(eval)? Constantv{PossibleValueKinds::NAC}: Constantv{eval};
  //   	}
  //   	else if (value1.isVaried() || value2.isVaried()) {
  //     		return Constantv{PossibleValueKinds::NAC};
  //   	}
  //   	else {
  //     	return Constantv{PossibleValueKinds::UNDEFINED};
  //   	}
 	// }

 // 	Constantv  ConstantAnalysis:: evaluateCast(llvm::CastInst& castOp, ConstantState& state)
 // 	{
	//     auto* op   = castOp.getOperand(0);
	//     auto value = getConstantValueFor(op, state);

	//     if (value.isConstant())
	//     {
	//       auto& layout = castOp.getModule()->getDataLayout();
	//       auto eval    = ConstantFoldCastOperand(castOp.getOpcode(), value.value,castOp.getDestTy(), layout);
	//       return llvm::isa<llvm::ConstantExpr>(eval)? Constantv{PossibleValueKinds::NAC}: Constantv{eval};
	//     }
	//     else
	//     {
	//       return Constantv{value.kind};
	//     }
	// }

	// void ConstantAnalysis:: op(llvm::Value& i, ConstantState& state)
	// {
	//     if (auto* constant = llvm::dyn_cast<llvm::Constant>(&i))
	//     {
	//       state[&i] = Constantv{constant};
	//     }
	//     else if (auto* binOp = llvm::dyn_cast<llvm::BinaryOperator>(&i))
	//     {
	//       state[binOp] = evaluateBinaryOperator(*binOp, state);
	//     }
	//     else if (auto* castOp = llvm::dyn_cast<llvm::CastInst>(&i))
	//     {
	//       state[castOp] = evaluateCast(*castOp, state);
	//     }
	//     else
	//     {
	//       state[&i].kind = PossibleValueKinds::NAC;
	//     }


	// Constantv ConstantAnalysis:: getConstantValueFor(llvm::Value* v, ConstantState& state)
 //  	}