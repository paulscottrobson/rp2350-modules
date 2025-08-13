// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      plot.c
//      Purpose :   Plot command handler.
//      Date :      13th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//                  Lennart Benschop
//
// *******************************************************************************************
// *******************************************************************************************

#include "graphics_module.h"
#include "graphics_module_local.h"

/**
 * @brief      Dispatch PLOT command
 *
 * @param[in]  cmd     Command (lower 3 bits stripped)
 * @param      xCoord  Array of x coordinates, [0] most recent
 * @param      yCoord  Array of y coordinates, [0] most recent
 */
void VDUPlotDispatch(int cmd,int *xCoord,int *yCoord) {
	int r,r2;
	// printf("%d %d,%d %d,%d %d,%d\n",cmd,xCoord[0],yCoord[0],xCoord[1],yCoord[1],xCoord[2],yCoord[2]);
	switch(cmd) {
		case 0:  																	// 0-31 Line drawers. Variants are set previously.
		case 8:
		case 16:
		case 24:
			VDUALine(xCoord[1],yCoord[1],xCoord[0],yCoord[0]);
			break;

		case 64:																	// 64-71 plot point
			VDUAPlot(xCoord[0],yCoord[0]);
			break;

		case 72:  																	// 72-79 horizontal flood fill
			VDUHorizontallFill(xCoord[0],yCoord[0],false);
			break;

		case 80:  																	// 80-87 filled triangle.
			VDUAFillTriangle(xCoord[2],yCoord[2],xCoord[1],yCoord[1],xCoord[0],yCoord[0]);
			break;

		case 88:  																	// 88-95 horizontal flood fill (right only)
			VDUHorizontallFill(xCoord[0],yCoord[0],true);
			break;

		case 96: 																	// 96-103 filled rectangle.
			VDUAFillRect(xCoord[0],yCoord[0],xCoord[1],yCoord[1]);
			break;

		case 144: 																	// 144-151 Outline circle
		case 152: 																	// 152-159 Filled circle
			r = abs(xCoord[0]-xCoord[1]);
			if (cmd == 144) {
				VDUAFrameEllipse(xCoord[1]-r,yCoord[1]-r,xCoord[1]+r,yCoord[1]+r);
			} else {
				VDUAFillEllipse(xCoord[1]-r,yCoord[1]-r,xCoord[1]+r,yCoord[1]+r);
			}
			break;
		case 192: 																	// 192-199 Outline ellipse
		case 200: 																	// 200-207 Filled ellipse
			r = abs(xCoord[1]-xCoord[2]);  											// Note, this does not allow angled ellipses, GXR does.
			r2 = abs(yCoord[0]-yCoord[2]);
			if (cmd == 192) {
				VDUAFrameEllipse(xCoord[2]-r,yCoord[1]-r2,xCoord[2]+r,yCoord[1]+r2);
			} else {
				VDUAFillEllipse(xCoord[2]-r,yCoord[1]-r2,xCoord[2]+r,yCoord[1]+r2);
			}
	}
}
