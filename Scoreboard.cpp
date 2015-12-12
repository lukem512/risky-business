// Luke Mitchell, 2015
// luke.mitchell.2011@my.bristol.ac.uk

// Scoreboarding Out-of-Order Algorithm
// Ref: https://en.wikipedia.org/wiki/Scoreboarding

bool Scoreboard::issue(uint8_t type, uint8_t opcode, uint8_t r1, uint8_t r2, uint8_t r3,
		int16_t im1, Register* pc, bool speculative) {

	// Get an available execution unit
	ExecutionUnit* eu = eum->getAvailableExecutionUnit();

	// Is the execution unit free?
	if (eu == NULL) {
		return false;
	}

	// Is the destination register free?
	if (Results[r1] != NULL) {
		return false;
	}

	// Issue the instruction to the EU
	eu->issue(type, opcode, r1, r2, r3, im, pc, speculative);

    Op[eu] = opcode;
    Type[eu] = type;
    Fi[eu] = r1;
 	Fj[eu] = r2;
	Fk[eu] = r3;

    Qj[eu] = Result[r2];
    Qk[eu] = Result[r3];
    Rj[eu] = Qj[eu] == NULL;
    Rk[eu] = Qk[eu] == NULL;

	Result[r1] = eu;

	return true;
}

bool Scoreboard::read(ExecutionUnit* eu) {
	// Wait for input registers
	// Then set them to unavailable
    switch (Type[eu]) {
    	case EU_ISSUE_O:
    	case EU_ISSUE_OR:
    		Rj[eu] = true;
    		Rk[eu] = true;
    		if (Op[eu] == OP_ST) {
    			if (!Rj[eu]) {
					return false;
				}
				Rj[eu] = false;
    		}
    		break;

    	case EU_ISSUE_ORR:
			if (!Rj[eu]) {
				return false;
			}
			Rj[eu] = false;
			Rk[eu] = true;
			if (Op[eu] == OP_STR) {
    			if (!Rk[eu]) {
					return false;
				}
				Rk[eu] = false;
    		}
    		break;

    	case EU_ISSUE_ORRR:
	    	if (!Rj[eu] && !Rk[eu]) {
				return false;
			}
    		Rj[eu] = false;
    		Rk[eu] = false;
	    	break;
    }
    return true;
}

bool Scoreboard::execute(ExecutionUnit* eu) {
	eu->tick();
	return (eu->ready && !eu->working);
}

// Delay until earlier instructions have completed their 
// read_operands stage
bool Scoreboard::write(ExecutionUnit* eu) {

	// Anything to write back?
	bool writeback;
    switch (Op[eu]) {
    	case OP_ST:
    	case OP_STR:
    		writeback = false;
    		break;

    	default:
    		writeback = true;
    		break;
    }

	if (Type[eu] == EU_ISSUE_O) {
		writeback = false;
	}

	// OK, do it!
    if (writeback) {
		// wait until (\forallf {(Fj[f]≠Fi[eu] OR Rj[f]=No) AND (Fk[f]≠Fi[eu] OR Rk[f]=No)})
		for (int i = 0; i < eus.size(); i++) {
			// Does eus[i] have Fj?
			if (Op[eus[i]] == OP_ST ||
				Type[eus[i]] == EU_ISSUE_ORR ||
				Type[eus[i]] == EU_ISSUE_ORRR) {
				if (Fj[eus[i]] != Fi[eu] || !Rj[eus[i]]) {
					// Does eus[i] have Fk?
					if (Op[eus[i]] == OP_STR ||
						Type[eus[i]] == EU_ISSUE_ORRR) {
						if (Fk[eus[i]] != Fi[eu] || !Rk[eus[i]]) {
							continue;
						}
					} else {
						continue;
					}
				}
			} else {
				continue;
			}
			return false;
		}
	}

    for (int i = 0; i < eus.size(); i++) {
    	if (Qj[eus[i]] == eu) {
    		Rj[eus[i]] = true;
    	}
    	if (Qk[eus[i]] == eu) {
    		Rk[eus[i]] = true;
    	} 
    }

    // NULL means no FU generates the register's result
    Result[Fi[eu]] = NULL;
    return true;
}