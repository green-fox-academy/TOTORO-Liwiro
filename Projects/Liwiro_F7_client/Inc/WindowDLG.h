/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __WINDOWDLG_H
#define __WINDOWDLG_H

void _cbDialog(WM_MESSAGE * pMsg);
WM_HWIN CreateWindow(void);
void MainTask(void);

extern uint8_t ctrl;

#endif /* __WINDOWDLG_H */
