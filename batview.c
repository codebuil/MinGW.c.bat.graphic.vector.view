#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_LINE_LENGTH 1024
#define MAX_FIELDS 100
int coords[324096];
int counts=0;
// Define a função de tratamento de mensagens da janela
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            
            // Define a cor de fundo da janela para azul
            HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 255));
            FillRect(hdc, &ps.rcPaint, hBrush);
            DeleteObject(hBrush);
            
            // Define a cor das linhas para branco
            HPEN hPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
            SelectObject(hdc, hPen);
            
            // Desenha as linhas com base nas coordenadas fornecidas
            
            int count = counts ;
            Polyline(hdc, (POINT*)coords, count / 2);
            
            // Libera os recursos utilizados
            DeleteObject(hPen);
            EndPaint(hWnd, &ps);
            break;
        }
        default:
            return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}

// Função principal do programa
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    LPWSTR* argList;
    int argCount;
    char filename[4096];
    char line[MAX_LINE_LENGTH];
    char *fields[MAX_FIELDS];
    int i=0;
    LPWSTR cmdLine = GetCommandLineW();
    argList = CommandLineToArgvW(cmdLine, &argCount);
    if (argCount<2) 
    {
        printf("give me the file name\r\n\r\n");
        exit(1);
    }
    WideCharToMultiByte(CP_UTF8, 0, argList[1], -1, filename, 2000, NULL, NULL );
    
    LocalFree(argList);
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error opening file %s\n", filename);
        return 1;
    }
        while (fgets(line, MAX_LINE_LENGTH, fp)) {
        // Replace newline with null terminator
        line[strcspn(line, "\n")] = '\0';

        // Split line into fields
        i = 0;
        fields[i] = strtok(line, ",");
        while (fields[i] != NULL && i < MAX_FIELDS) {
            i++;
            fields[i] = strtok(NULL, ",");
        }
        

        // Print fields separated by tabs

        if(i>4) {
            for (int j = 1; j < i; j++) {
                 coords[counts]=atoi(fields[j]);
                 counts++;
            }
        }
        printf("\n");
    }

    fclose(fp);
    // Define as propriedades da janela
    const char CLASS_NAME[] = "MyClass";
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    RegisterClass(&wc);
    
    // Cria a janela
    HWND hWnd = CreateWindowEx(
        0,                          // Estilo estendido
        CLASS_NAME,                 // Nome da classe
        "Janela Azul com Linhas",   // Texto da janela
        WS_OVERLAPPEDWINDOW,        // Estilo da janela
        CW_USEDEFAULT,              // Posição x
        CW_USEDEFAULT,              // Posição y
        CW_USEDEFAULT,              // Largura
        CW_USEDEFAULT,              // Altura
        NULL,                       // Janela pai
        NULL,                       // Menu
        hInstance,                  // Instância do programa
        NULL                        // Argumentos adicionais
    );
    
    // Exibe a janela
    ShowWindow(hWnd, nCmdShow);
    
    // Loop de mensagens da janela
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    return 0;
}

