#include <graphics.h>
#include <conio.h>
#include <mmsystem.h>
#include <vector>
#include <queue>
#include <ctime>
#include <cmath>
#include <algorithm>
#pragma comment(lib, "winmm.lib")

#include "play.h"

// 游戏设置
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int LANE_WIDTH = 90;
const int LANE_COUNT = 4;
const int JUDGELINE_Y = 600;
const int NOTE_SPEED = 5;
const int GOOD_MS = 1000;        // 音符存在时间（毫秒）
const int MAX_SCORE = 1000000;

// 判定结果枚举
enum Judgment {
    NONE,
    PERFECT,
    MISS
};

// 音符结构体
struct Note {
    int lane;            // 轨道位置 (0-3)
    double time;         // 出现时间（毫秒）
    bool processed;      // 是否已判定

    Note(int l, double t)
        : lane(l), time(t), processed(false) {}
};

// 游戏状态结构体
struct GameState {
    std::vector<Note> notes;
    std::queue<Judgment> judgments;
    int combo = 0;
    int maxCombo = 0;
    int score = 0;
    int noteIndex = 0;
    bool gameOver = false;
} gameState;

// 初始化游戏
void initGame() {
    srand((unsigned int)time(nullptr));

    // 生成 TAP 音符
    for (int i = 0; i < 50; i++) {
        int lane = rand() % LANE_COUNT;
        double time = i * 500 + rand() % 200;
        gameState.notes.push_back(Note(lane, time));
    }

    // 按时间排序
    std::sort(gameState.notes.begin(), gameState.notes.end(), [](const Note& a, const Note& b) {
        return a.time < b.time;
        });
}

// 判定函数
void checkNotes(double currentTime) {
    static bool keyPressed[4] = { false };  // 记录按键是否已触发过

    for (int i = 0; i < gameState.notes.size(); ++i) {
        Note& note = gameState.notes[i];

        if (note.processed) continue;

        // 判断是否在有效时间内
        if (currentTime >= note.time && currentTime <= note.time + GOOD_MS) {
            bool pressed = false;
            int keyIndex = -1;

            // 检查对应按键是否按下
            if (note.lane == 0 && (GetAsyncKeyState('D') & 0x8000)) {
                keyIndex = 0;
                pressed = true;
            } else if (note.lane == 1 && (GetAsyncKeyState('F') & 0x8000)) {
                keyIndex = 1;
                pressed = true;
            } else if (note.lane == 2 && (GetAsyncKeyState('J') & 0x8000)) {
                keyIndex = 2;
                pressed = true;
            } else if (note.lane == 3 && (GetAsyncKeyState('K') & 0x8000)) {
                keyIndex = 3;
                pressed = true;
            }

            if (pressed && !keyPressed[keyIndex]) {
                // 判定为 PERFECT
                gameState.judgments.push(PERFECT);
                gameState.score += MAX_SCORE / gameState.notes.size();
                gameState.combo++;
                note.processed = true;
                keyPressed[keyIndex] = true;  // 标记已触发
            }
        } else if (currentTime > note.time + GOOD_MS + 50) {
            // 超时未按
            gameState.judgments.push(MISS);
            gameState.combo = 0;
            note.processed = true;
        }
    }

    // 每帧结束后重置按键状态
    for (int i = 0; i < 4; ++i) {
        if (keyPressed[i] && (
            (i == 0 && !(GetAsyncKeyState('D') & 0x8000)) ||
            (i == 1 && !(GetAsyncKeyState('F') & 0x8000)) ||
            (i == 2 && !(GetAsyncKeyState('J') & 0x8000)) ||
            (i == 3 && !(GetAsyncKeyState('K') & 0x8000))
            )) {
            keyPressed[i] = false;
        }
    }

    if (gameState.combo > gameState.maxCombo)
        gameState.maxCombo = gameState.combo;
}

// 绘制游戏界面
void drawGame(double currentTime) {
    cleardevice();
    setbkcolor(BLACK);
    cleardevice();

    // 绘制轨道
    setlinecolor(WHITE);
    setlinestyle(PS_SOLID, 2);
    for (int i = 0; i <= LANE_COUNT; ++i) {
        line(SCREEN_WIDTH / 2 - LANE_WIDTH * (LANE_COUNT / 2) + i * LANE_WIDTH, 0,
            SCREEN_WIDTH / 2 - LANE_WIDTH * (LANE_COUNT / 2) + i * LANE_WIDTH, SCREEN_HEIGHT);
    }

    // 判定线
    line(SCREEN_WIDTH / 2 - LANE_WIDTH * (LANE_COUNT / 2), JUDGELINE_Y,
        SCREEN_WIDTH / 2 + LANE_WIDTH * (LANE_COUNT / 2), JUDGELINE_Y);

    // 绘制音符
    for (Note& note : gameState.notes) {
        if (note.processed) continue;

        double timeDiff = note.time - currentTime;
        int y = JUDGELINE_Y - (int)(timeDiff * NOTE_SPEED / 10);

        if (y > -30 && y < SCREEN_HEIGHT) {
            int x = SCREEN_WIDTH / 2 - LANE_WIDTH * (LANE_COUNT / 2) + note.lane * LANE_WIDTH + LANE_WIDTH / 2;

            setfillcolor(WHITE);
            solidcircle(x, y, 15);
        }
    }

    // 显示判定结果
    if (!gameState.judgments.empty()) {
        Judgment result = gameState.judgments.front();
        if ((GetTickCount() % 2000) > 1000) {
            gameState.judgments.pop();
        } else {
            const TCHAR* resultText;
            int color;
            switch (result) {
            case PERFECT: resultText = _T("PERFECT"); color = RGB(255, 215, 0); break;
            case MISS: resultText = _T("MISS"); color = RGB(255, 99, 71); break;
            default: resultText = _T(""); color = WHITE; break;
            }

            settextstyle(30, 0, _T("Consolas"));
            settextcolor(color);
            outtextxy(SCREEN_WIDTH / 2 - 100, JUDGELINE_Y - 60, resultText);
        }
    }

    // 显示连击数
    TCHAR comboText[50];
    _stprintf_s(comboText, _T("COMBO: %d"), gameState.combo);
    settextstyle(25, 0, _T("Consolas"));
    settextcolor(WHITE);
    outtextxy(SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT - 80, comboText);

    // 最大连击
    TCHAR maxComboText[50];
    _stprintf_s(maxComboText, _T("MAX COMBO: %d"), gameState.maxCombo);
    outtextxy(SCREEN_WIDTH / 2 - 70, SCREEN_HEIGHT - 50, maxComboText);

    // 得分
    TCHAR scoreText[50];
    _stprintf_s(scoreText, _T("SCORE: %d"), gameState.score);
    outtextxy(SCREEN_WIDTH - 200, 20, scoreText);

    // 双缓冲刷新
    EndBatchDraw();
}

// 显示游戏结束画面
void showGameOver() {
    settextstyle(40, 0, _T("微软雅黑"));
    settextcolor(WHITE);
    outtextxy(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 40, _T("GAME OVER"));

    TCHAR scoreText[50];
    _stprintf_s(scoreText, _T("FINAL SCORE: %d"), gameState.score);
    outtextxy(SCREEN_WIDTH / 2 - 120, SCREEN_HEIGHT / 2 + 20, scoreText);

    TCHAR maxComboText[50];
    _stprintf_s(maxComboText, _T("MAX COMBO: %d"), gameState.maxCombo);
    outtextxy(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 70, maxComboText);
}

// 倒计时函数
void showCountdown() {
    settextstyle(60, 0, _T("Consolas"));
    settextcolor(WHITE);

    const TCHAR* countdown[] = { _T("3"), _T("2"), _T("1"), _T("GO!") };
    for (int i = 0; i < 4; ++i) {
        cleardevice();
        outtextxy(SCREEN_WIDTH / 2 - 20, SCREEN_HEIGHT / 2 - 30, countdown[i]);
        EndBatchDraw();
        Sleep(1000);  // 等待1秒
    }
}

// 主函数
int play() {
    initgraph(SCREEN_WIDTH, SCREEN_HEIGHT);
    HWND hwnd = GetHWnd();
    SetWindowText(hwnd, _T("4K下落式音游"));

    BeginBatchDraw();  // 启用双缓冲绘图

    initGame();
    showCountdown();  // 显示倒计时
    clock_t startTime = clock();

    while (!gameState.gameOver) {
        double currentTime = (double)(clock() - startTime) / CLOCKS_PER_SEC * 1000;  // 毫秒

        // 检查是否所有音符都已处理
        bool allProcessed = true;
        for (const auto& note : gameState.notes) {
            if (!note.processed && currentTime < note.time + 2000) {
                allProcessed = false;
                break;
            }
        }

        if (allProcessed && gameState.judgments.empty()) {
            gameState.gameOver = true;
            continue;
        }

        // 检查按键并判定
        checkNotes(currentTime);

        drawGame(currentTime);

        if (!gameState.judgments.empty() && GetTickCount() % 2000 > 1000) {
            gameState.judgments.pop();
        }

        Sleep(16);  // 控制帧率
    }

    showGameOver();
    while (true) {
        if (_kbhit() && _getch() == 27) break;
    }

    closegraph();
    return 0;
}