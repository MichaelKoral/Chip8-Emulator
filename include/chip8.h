#define STACK_SIZE 4096
#define NUMBER_OF_REGISTERS 0xFu
#define PROGRAM_START 0x200u
typedef char byte;
struct chip_state {
  byte stack[STACK_SIZE];
  byte general_regs[NUMBER_OF_REGISTERS];  
  byte I_reg[2]; 
  byte DT_reg;
  byte ST_reg;
  byte PC_reg;
  byte SP_reg;
};
typedef struct chip_state chip_state;
