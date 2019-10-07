

#include "main.h"
#include "manager.h"
#include "resource.h"
#include <CommCtrl.h>
#pragma comment(lib,"Comctl32.lib")

const char* CLASS_NAME = "DX11AppClass";
const char* WINDOW_NAME = "DX11";


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT  CALLBACK  DlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);

HWND g_Window;

HWND GetWindow()
{
	return g_Window;
}


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	InitCommonControls();   //コモンコントロールを初期化
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		MAKEINTRESOURCE(IDR_MENU1),
		CLASS_NAME,
		NULL
	};

	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// ウィンドウの作成
	g_Window = CreateWindowEx(0,
		CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		(SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME) * 2),
		(SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION)),
		NULL,
		NULL,
		hInstance,
		NULL);


	// 初期化処理(ウィンドウを作成してから行う)
	CManager::Init();


	// ウインドウの表示(初期化処理の後に行う)
	ShowWindow(g_Window, nCmdShow);
	UpdateWindow(g_Window);

	// アクセラレータテーブル読み込み
	HACCEL hacc;
	hacc = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));
	//モードレスダイアログ作成
	/*ghDlg = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), g_hWnd, (DLGPROC)DlgProc);
	if (!ghDlg) return FALSE;*/
	//ShowWindow(ghDlg, SW_SHOW);   //ダイアログを表示

	//フレームカウント初期化
	DWORD dwExecLastTime;
	DWORD dwCurrentTime;
	timeBeginPeriod(1);
	dwExecLastTime = timeGetTime();
	dwCurrentTime = 0;


	// メッセージループ
	MSG msg;
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (TranslateAccelerator(GetWindow(), hacc, &msg))
				continue;
			if (msg.message == WM_QUIT)
			{// PostQuitMessage()が呼ばれたらループ終了
				break;
			}
			else
			{
				// メッセージの翻訳とディスパッチ
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			dwCurrentTime = timeGetTime();

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{
				dwExecLastTime = dwCurrentTime;

				// 更新処理
				CManager::Update();

				// 描画処理
				CManager::Draw();
			}
		}
	}

	timeEndPeriod(1);				// 分解能を戻す

	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// 終了処理
	CManager::Uninit();

	return (int)msg.wParam;
}


//=============================================================================
// ウインドウプロシージャ
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_40001:
			// 新規作成
		{
			break;
		}
		case ID_40002:
			// 終了
			DestroyWindow(hWnd);
			break;
		}
		
	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//Dialog Box用 CALLBACK関数
//LRESULT  CALLBACK  DlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
//{
//
//	switch (msg) {
//		//----初期化----
//	case WM_INITDIALOG: {
//		//トラックバーのハンドルを取得
//		HWND hSlider = GetDlgItem(hDlg, IDC_SLIDER1);
//
//		//スライダのとる値を0～255に
//		SendMessage(hSlider, TBM_SETRANGE, FALSE, MAKELPARAM(0, 255));
//		SendMessage(hSlider, TBM_SETPAGESIZE, 0, 32); //ページサイズを32に
//		SendMessage(hSlider, TBM_SETTICFREQ, 32, 0);  //目盛り間隔を32に
//
//		return TRUE;
//
//	}
//	case WM_HSCROLL:
//		//スライダの位置を取得(戻り値で値をゲット)
//		Manager::model->count = SendMessage((HWND)lParam, TBM_GETPOS, 0, 0);
//		//親ウインドウ再描画
//		InvalidateRect(GetParent(hDlg), NULL, FALSE);
//		UpdateWindow(GetParent(hDlg));
//		return TRUE;
//	case WM_COMMAND:
//	{
//		//HWND hButton = GetDlgItem(hDlg, IDC_BUTTONPAUSE);
//		switch (LOWORD(wParam))
//		{
//		case IDC_BUTTONPAUSE:
//			Manager::Mine()->model->IsStop = true;
//			break;
//		case IDC_BUTTONPlay:
//			Manager::Mine()->model->IsStop = false;
//			break;
//		case IDC_BUTTONSTOP:
//			Manager::Mine()->model->IsStop = true;
//			Manager::Mine()->model->count = 0;
//			break;
//		default:
//			break;
//		}
//
//		HWND hCheck = GetDlgItem(hDlg, IDC_CHECK1);
//		if (BST_CHECKED == SendMessage(hCheck, BM_GETCHECK, 0, 0))
//			Manager::model->IsWeight = true;
//		else
//			Manager::model->IsWeight = false;
//	}
//	return TRUE;
//	case WM_CLOSE:
//		//DestroyWindow(GetParent(hDlg));
//		DestroyWindow(hDlg);
//		ghDlg = NULL;
//		return TRUE;
//
//
//		//アクセラレータのためには必要不可欠
//	   //return TRUE;
//	}
//	return FALSE;
//}