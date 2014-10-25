#include <complex>
#include <windows.h>
#include <vector>
#include <stdio.h>
#include <omp.h>
#include <windowsx.h>
using std::complex;
using std::vector;

#include "brot.h"

Brot* brot = NULL;
HWND hwnd = NULL;
POINT dragstart = {0};

LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch(Message) {
		case WM_PAINT: {
			
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd,&ps);
			
			RECT rcClient;
			GetClientRect(hwnd,&rcClient);
			
			// Teken de achtergrond
			SelectObject(hdc,GetStockObject(WHITE_PEN));
			SelectObject(hdc,GetStockObject(WHITE_BRUSH));
			Rectangle(hdc,0,0,rcClient.right,rcClient.bottom);
			
			// Teken de gevonden punten
			for(int x = 0;x < rcClient.right;x++) {
				for(int y = 0;y < rcClient.bottom;y++) {
					int thisiterations = brot->points[x][y];
					if(thisiterations > 0) {
						
						RECT thisrect = {
							x,
							y,
							x + 1,
							y + 1,
						};
					
						SetBkColor(hdc,RGB(0,thisiterations * 255.0f / 100.0f,0));
						ExtTextOut(hdc,0,0,ETO_OPAQUE,&thisrect,"",0,NULL);
					}
				}
			}
			
			EndPaint(hwnd,&ps);
			break;
		}
		case WM_LBUTTONDOWN: {
			dragstart.x = GET_X_LPARAM(lParam);
			dragstart.y = GET_Y_LPARAM(lParam);
			break;
		}
		case WM_LBUTTONUP: {
			POINT dragdir;
			dragdir.x = GET_X_LPARAM(lParam) - dragstart.x;
			dragdir.y = GET_Y_LPARAM(lParam) - dragstart.y;
			
			brot->SetViewport(
				brot->left - dragdir.x * brot->xpixel,
				brot->top - dragdir.y * brot->ypixel,
				brot->right - dragdir.x * brot->xpixel,
				brot->bottom - dragdir.y * brot->ypixel);
			brot->Fill();
			InvalidateRect(hwnd,NULL,false);
			break;
		}
		case WM_SIZE: {
			if(brot) {
				
				RECT rcClient;
				GetClientRect(hwnd,&rcClient);
				
				brot->SetSize(rcClient.right,rcClient.bottom);
				brot->Fill();
				InvalidateRect(hwnd,NULL,false);
			}
			break;
		}
		case WM_MOUSEWHEEL: {
			
			break;
		}
		case WM_DESTROY: {
			PostQuitMessage(0);
			break;
		}
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wc;
	MSG Msg;

	memset(&wc,0,sizeof(wc));
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.lpfnWndProc	 = WndProc;
	wc.hInstance	 = hInstance;
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszClassName = "WindowClass";
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION);

	if(!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}
	
	RECT result = {0,0,512 * 2,512};
	AdjustWindowRect(&result,WS_VISIBLE|WS_OVERLAPPEDWINDOW,0);
	hwnd = CreateWindowEx(WS_EX_COMPOSITED,"WindowClass","Mandelbrot",WS_VISIBLE|WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		result.right - result.left,
		result.bottom - result.top,
		NULL,NULL,hInstance,NULL);

	if(hwnd == NULL) {
		MessageBox(NULL, "Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}
	
	// Pas maken als window klaar is met sizen
	brot = new Brot();
	brot->SetSize(512 * 2,512);
	brot->SetViewport(-2,-1,1,1);
	brot->Fill();

	while(GetMessage(&Msg, NULL, 0, 0) > 0) {
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}
