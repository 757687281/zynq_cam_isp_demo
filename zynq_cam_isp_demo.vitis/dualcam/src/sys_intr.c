#include "xparameters.h"
#include "sys_intr.h"
#include "xil_exception.h"

#ifdef XPAR_INTC_0_DEVICE_ID

static XIntc axiIntc;
static XScuGic intc;

//�жϳ�ʼ��
int sys_intr_init(void)
{
    XIntc_Initialize(&axiIntc, XPAR_AXI_INTC_0_DEVICE_ID);
    XIntc_Start(&axiIntc, XIN_REAL_MODE);

    int status;
    //��ʼ���жϿ�����
    XScuGic_Config *intc_cfg;
    intc_cfg = XScuGic_LookupConfig(XPAR_SCUGIC_SINGLE_DEVICE_ID);
    if (NULL == intc_cfg) {
        return XST_FAILURE;
	}
    status = XScuGic_CfgInitialize(&intc, intc_cfg, intc_cfg->CpuBaseAddress);
    if (status != XST_SUCCESS)
        return XST_FAILURE;

    XScuGic_Connect(&intc, XPS_FPGA0_INT_ID, (Xil_ExceptionHandler) XIntc_InterruptHandler, (void*)&axiIntc);
    XScuGic_Enable(&intc, XPS_FPGA0_INT_ID);

    //���ò����ж��쳣������
    Xil_ExceptionInit();
    Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT, (Xil_ExceptionHandler)XScuGic_InterruptHandler, (void*)&intc);
    Xil_ExceptionEnable();

    return XST_SUCCESS;
}

XIntc* sys_intr_inst()
{
	return &axiIntc;
}

#else

static XScuGic intc;

//�жϳ�ʼ��
int sys_intr_init(void)
{
    int status;
    //��ʼ���жϿ�����
    XScuGic_Config *intc_cfg;
    intc_cfg = XScuGic_LookupConfig(XPAR_SCUGIC_SINGLE_DEVICE_ID);
    if (NULL == intc_cfg) {
        return XST_FAILURE;
	}
    status = XScuGic_CfgInitialize(&intc, intc_cfg, intc_cfg->CpuBaseAddress);
    if (status != XST_SUCCESS)
        return XST_FAILURE;

    //���ò����ж��쳣������
    Xil_ExceptionInit();
    Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT, (Xil_ExceptionHandler)XScuGic_InterruptHandler, (void*)&intc);
    Xil_ExceptionEnable();

    return XST_SUCCESS;
}

XScuGic* sys_intr_inst()
{
	return &intc;
}

#endif
