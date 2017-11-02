#ifndef CAN_h_
#define CAN_h_


unsigned char rxflag;
typedef struct
{
	unsigned int id;
	unsigned int length;
	unsigned int data[8];
} CAN_message;
//
//typedef struct {
	//union {
		//unsigned char idBytes[2];
		//unsigned int id;
	//};
	//unsigned char length;
	//union {
		//unsigned char data[8];
		//unsigned int position[4];
	//};
//} CAN_message;

void CAN_init();
void CAN_send(CAN_message * msg);
int Can_trans_compl();

void CAN_Int_Reset();

//unsigned char CAN_read(); 
void CAN_read2();


#endif