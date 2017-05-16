//
//  main.cpp
//  SDL_Game
//
//  Created by Đỗ Đình Hiếu on 17/Apr/2017.
//  Copyright © 2017 Đỗ Đình Hiếu. All rights reserved.
//

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <vector>

using namespace std;

/**
class everything {
    public:
    
    
};
*/

const int SCREEN_WIDTH = 1600;
const int SCREEN_HEIGHT = 1000;
const string WINDOW_TITLE = "Do Dinh Hieu";

int minDistance = 400, maxDistance = 900; // Khoảng cách tối thiểu, tối đa giữa các cây
long highScore = 0;

void logSDLError(std::ostream& os, const std::string &msg, bool fatal = false);
void initSDL(SDL_Window* &window, SDL_Renderer* &renderer);
void quitSDL(SDL_Window* window, SDL_Renderer* renderer);
void waitUntilESC();

SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren);
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y);
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h);

int createRandomNumber(int min, int max) {
    return min + rand() % (max - min + 1);
}

void changeParameters(int xTree[], int treeNumber[], int &groundPos, long &score, int &step);

int getTreeWidth(SDL_Texture* tree);
int getTextureHeight(SDL_Texture* tree);

bool checkIfDie(SDL_Texture* element, int xElement, int yElement, SDL_Texture* tree, int xTree, int yTree);
bool ifPlayAgain();
void printScore(SDL_Renderer* renderer, long &score);

SDL_Event waitPressedKey() { // Wait for starting or exit
    SDL_Event e;
    while (true) {
        if (SDL_WaitEvent(&e) != 0) {
            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_SPACE || e.key.keysym.sym == SDLK_ESCAPE) return e;
            }
        }
        SDL_Delay(100);
    }
}

int main() {
    
    srand(time(0));
    SDL_Window* window;
    SDL_Renderer* renderer;
    initSDL(window, renderer);
    TTF_Init();
    
    SDL_Event e;
    
    long score = 0;
    int step = 5;    // bước dịch chuyển
    const int delayTime = 3;
    const int xElement = SCREEN_WIDTH / 4;
    int yElement = SCREEN_HEIGHT / 2;
    const int yGround = SCREEN_HEIGHT / 2 + 75;
    
    
    vector <SDL_Texture* > element;
    element.push_back(loadTexture("/Users/hieudd/Desktop/Images/step1.bmp", renderer));
    element.push_back(loadTexture("/Users/hieudd/Desktop/Images/step2.bmp", renderer));
    element.push_back(loadTexture("/Users/hieudd/Desktop/Images/standing.bmp", renderer));
    element.push_back(loadTexture("/Users/hieudd/Desktop/Images/dead.bmp", renderer));
    
    vector <SDL_Texture* > tree;
    tree.push_back(loadTexture("/Users/hieudd/Desktop/Images/tree0.bmp", renderer));
    tree.push_back(loadTexture("/Users/hieudd/Desktop/Images/tree1.bmp", renderer));
    tree.push_back(loadTexture("/Users/hieudd/Desktop/Images/tree2.bmp", renderer));
    tree.push_back(loadTexture("/Users/hieudd/Desktop/Images/tree3.bmp", renderer));
    tree.push_back(loadTexture("/Users/hieudd/Desktop/Images/tree4.bmp", renderer));
    tree.push_back(loadTexture("/Users/hieudd/Desktop/Images/tree5.bmp", renderer));
    tree.push_back(loadTexture("/Users/hieudd/Desktop/Images/tree6.bmp", renderer));
    tree.push_back(loadTexture("/Users/hieudd/Desktop/Images/tree7.bmp", renderer));
    tree.push_back(loadTexture("/Users/hieudd/Desktop/Images/tree8.bmp", renderer));
    
    SDL_Texture* lying = loadTexture("/Users/hieudd/Desktop/Images/lying.bmp", renderer);
    
    SDL_Texture* ground = loadTexture("/Users/hieudd/Desktop/Images/ground.bmp", renderer);
    
    vector <SDL_Texture* > start;
    start.push_back(loadTexture("/Users/hieudd/Desktop/Images/start.bmp", renderer));
    start.push_back(loadTexture("/Users/hieudd/Desktop/Images/instruction.bmp", renderer));
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);   // white
    
    bool running = false;
    
    SDL_RenderClear(renderer);
    renderTexture(start[0], renderer, 0, 0);
    SDL_RenderPresent(renderer);
    e = waitPressedKey();
    if (e.key.keysym.sym == SDLK_SPACE) running = true;
    else if (e.key.keysym.sym == SDLK_ESCAPE) {
        SDL_Quit();
        return 0;
    }
    
    SDL_RenderClear(renderer);
    renderTexture(start[1], renderer, 0, 0);
    SDL_RenderPresent(renderer);
    e = waitPressedKey();
    if (e.key.keysym.sym == SDLK_SPACE) running = true;
    else if (e.key.keysym.sym == SDLK_ESCAPE) {
        SDL_Quit();
        return 0;
    }

    
    //  up
    for (int j = 1, stepJump = 10; j <= 25 && running; j ++) {
        yElement -= stepJump;
        if (j == 5) stepJump = 8;
        if (j == 15) stepJump = 5;
        
        SDL_Delay(delayTime);
        SDL_RenderClear(renderer);
        renderTexture(element[2], renderer, xElement, yElement);
        SDL_RenderPresent(renderer);
    }
    //  delay in the air
    for (int j = 0; j < 5; j ++) {
        SDL_Delay(delayTime);
        SDL_RenderClear(renderer);
        renderTexture(element[2], renderer, xElement, yElement);
        SDL_RenderPresent(renderer);
    }
    //down
    for (int j = 1, stepJump = 5; j <= 25 && running; j ++) {
        yElement += stepJump;
        if (j == 10) stepJump = 8;
        if (j == 20) stepJump = 10;
        
        SDL_Delay(delayTime);
        SDL_RenderClear(renderer);
        renderTexture(element[2], renderer, xElement, yElement);
        SDL_RenderPresent(renderer);
    }
    
    
    while (running) {
        int treeNumber[3],
            xTree[3],
            yTree[3],
            groundPos = 0;
        
        xTree[0] = SCREEN_WIDTH;
        xTree[1] = xTree[0] + createRandomNumber(400, 900);
        xTree[2] = xTree[1] + createRandomNumber(400, 900);
        
        
        treeNumber[0] = rand() % 9;
        treeNumber[1] = rand() % 9;
        treeNumber[2] = rand() % 9;
        
        yTree[0] = yGround + 15 - getTextureHeight(tree[treeNumber[0]]);
        yTree[1] = yGround + 15 - getTextureHeight(tree[treeNumber[1]]);
        yTree[2] = yGround + 15 - getTextureHeight(tree[treeNumber[2]]);
        
        for (int i = 0; running; i ++) {
            bool isJumping = false;
            bool isLying = false;
            
            
            SDL_Delay(delayTime);
            SDL_RenderClear(renderer);
            renderTexture(ground, renderer, groundPos, yGround, SCREEN_WIDTH, 20);
            renderTexture(ground, renderer, groundPos + SCREEN_WIDTH, yGround, SCREEN_WIDTH, 20);
            printScore(renderer, score);
            renderTexture(tree[treeNumber[0]], renderer, xTree[0], yGround + 15 - getTextureHeight(tree[treeNumber[0]]));
            renderTexture(tree[treeNumber[1]], renderer, xTree[1], yGround + 15 - getTextureHeight(tree[treeNumber[1]]));
            renderTexture(tree[treeNumber[2]], renderer, xTree[2], yGround + 15 - getTextureHeight(tree[treeNumber[2]]));
            renderTexture(element[i / 10 % 2], renderer, xElement, yElement);
            SDL_RenderPresent(renderer);
            
            
            for (int tmp = 0; tmp < 3; tmp ++) {
                if (checkIfDie(element[2], xElement, yElement, tree[treeNumber[tmp]], xTree[tmp], yGround + 15 - getTextureHeight(tree[treeNumber[tmp]]))) {
                    if (ifPlayAgain()) {
                        //reset thong so ban dau
                        step = 5;
                        score = 0;
                        treeNumber[tmp] = rand() % 9;
                        xTree[tmp] = xTree[(tmp+2) % 3] + createRandomNumber(minDistance, maxDistance);
                    }
                    else running = false;
                }
            }
            
            changeParameters(xTree, treeNumber, groundPos, score, step);
            
            if (i == 20) i = 0;
            
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) return 0;
                
                if (e.type == SDL_KEYDOWN) {
                    if (e.key.keysym.sym == SDLK_ESCAPE) running = false;
                    if (((e.key.keysym.sym == SDLK_SPACE || e.key.keysym.sym == SDLK_UP)))  isJumping = true;
                    else if (e.key.keysym.sym == SDLK_DOWN) isLying = true;
                }
            }
            // isJump == true thì thực hiện jump
            if (isJumping) {   // jump
                //  up
                for (int j = 1, stepJump = 10; j <= 25 && running; j ++) {
                    yElement -= stepJump;
                    if (j == 5) stepJump = 8;
                    if (j == 15) stepJump = 5;
                    
                    SDL_Delay(delayTime);
                    SDL_RenderClear(renderer);
                    renderTexture(ground, renderer, groundPos, yGround, SCREEN_WIDTH, 20);
                    renderTexture(ground, renderer, groundPos + SCREEN_WIDTH, yGround, SCREEN_WIDTH, 20);
                    printScore(renderer, score);
                    renderTexture(tree[treeNumber[0]], renderer, xTree[0], yGround + 15 - getTextureHeight(tree[treeNumber[0]]));
                    renderTexture(tree[treeNumber[1]], renderer, xTree[1], yGround + 15 - getTextureHeight(tree[treeNumber[1]]));
                    renderTexture(tree[treeNumber[2]], renderer, xTree[2], yGround + 15 - getTextureHeight(tree[treeNumber[2]]));
                    renderTexture(element[2], renderer, xElement, yElement);
                    SDL_RenderPresent(renderer);
                    
                    for (int tmp = 0; tmp < 3; tmp ++) {
                        if (checkIfDie(element[2], xElement, yElement, tree[treeNumber[tmp]], xTree[tmp], yGround + 15 - getTextureHeight(tree[treeNumber[tmp]]))) {
                            if (ifPlayAgain()) {
                                //reset thong so ban dau
                                step = 5;
                                score = 0;
                                treeNumber[tmp] = rand() % 9;
                                xTree[tmp] = xTree[(tmp+2) % 3] + createRandomNumber(minDistance, maxDistance);
                            }
                            else running = false;
                        }
                    }
                    
                    changeParameters(xTree, treeNumber, groundPos, score, step);
                    
                    yTree[0] = yGround + 15 - getTextureHeight(tree[treeNumber[0]]);
                    yTree[1] = yGround + 15 - getTextureHeight(tree[treeNumber[1]]);
                    yTree[2] = yGround + 15 - getTextureHeight(tree[treeNumber[2]]);
                    
                }
                
                //  delay in the air
                for (int j = 0; j < 0; j ++) {
                    SDL_Delay(delayTime);
                    SDL_RenderClear(renderer);
                    renderTexture(ground, renderer, groundPos, yGround, SCREEN_WIDTH, 20);
                    renderTexture(ground, renderer, groundPos + SCREEN_WIDTH, yGround, SCREEN_WIDTH, 20);
                    printScore(renderer, score);
                    renderTexture(tree[treeNumber[0]], renderer, xTree[0], yGround + 15 - getTextureHeight(tree[treeNumber[0]]));
                    renderTexture(tree[treeNumber[1]], renderer, xTree[1], yGround + 15 - getTextureHeight(tree[treeNumber[1]]));
                    renderTexture(tree[treeNumber[2]], renderer, xTree[2], yGround + 15 - getTextureHeight(tree[treeNumber[2]]));
                    renderTexture(element[2], renderer, xElement, yElement);
                    SDL_RenderPresent(renderer);
                    
                    changeParameters(xTree, treeNumber, groundPos, score, step);
                    
                    yTree[0] = yGround + 15 - getTextureHeight(tree[treeNumber[0]]);
                    yTree[1] = yGround + 15 - getTextureHeight(tree[treeNumber[1]]);
                    yTree[2] = yGround + 15 - getTextureHeight(tree[treeNumber[2]]);
                }
                
                //down
                for (int j = 1, stepJump = 5; j <= 25 && running; j ++) {
                    yElement += stepJump;
                    if (j == 10) stepJump = 8;
                    if (j == 20) stepJump = 10;
                    
                    SDL_Delay(delayTime);
                    SDL_RenderClear(renderer);
                    renderTexture(ground, renderer, groundPos, yGround, SCREEN_WIDTH, 20);
                    renderTexture(ground, renderer, groundPos + SCREEN_WIDTH, yGround, SCREEN_WIDTH, 20);
                    printScore(renderer, score);
                    renderTexture(tree[treeNumber[0]], renderer, xTree[0], yGround + 15 - getTextureHeight(tree[treeNumber[0]]));
                    renderTexture(tree[treeNumber[1]], renderer, xTree[1], yGround + 15 - getTextureHeight(tree[treeNumber[1]]));
                    renderTexture(tree[treeNumber[2]], renderer, xTree[2], yGround + 15 - getTextureHeight(tree[treeNumber[2]]));
                    renderTexture(element[2], renderer, xElement, yElement);
                    SDL_RenderPresent(renderer);
                    
                    for (int tmp = 0; tmp < 3; tmp ++) {
                        if (checkIfDie(element[2], xElement, yElement, tree[treeNumber[tmp]], xTree[tmp], yGround + 15 - getTextureHeight(tree[treeNumber[tmp]]))) {
                            if (ifPlayAgain()) {
                                //reset thong so ban dau
                                step = 5;
                                score = 0;
                                treeNumber[tmp] = rand() % 9;
                                xTree[tmp] = xTree[(tmp+2) % 3] + createRandomNumber(minDistance, maxDistance);
                            }
                            else running = false;
                        }
                    }
                    
                    changeParameters(xTree, treeNumber, groundPos, score, step);
                    
                    yTree[0] = yGround + 15 - getTextureHeight(tree[treeNumber[0]]);
                    yTree[1] = yGround + 15 - getTextureHeight(tree[treeNumber[1]]);
                    yTree[2] = yGround + 15 - getTextureHeight(tree[treeNumber[2]]);
                }
            }
            
            if (isLying) {
                for (int j = 1; j <= 25 && running; j ++) {
                    SDL_Delay(delayTime);
                    SDL_RenderClear(renderer);
                    renderTexture(ground, renderer, groundPos, yGround, SCREEN_WIDTH, 20);
                    renderTexture(ground, renderer, groundPos + SCREEN_WIDTH, yGround, SCREEN_WIDTH, 20);
                    printScore(renderer, score);
                    renderTexture(tree[treeNumber[0]], renderer, xTree[0], yGround + 15 - getTextureHeight(tree[treeNumber[0]]));
                    renderTexture(tree[treeNumber[1]], renderer, xTree[1], yGround + 15 - getTextureHeight(tree[treeNumber[1]]));
                    renderTexture(tree[treeNumber[2]], renderer, xTree[2], yGround + 15 - getTextureHeight(tree[treeNumber[2]]));
                    renderTexture(lying, renderer, xElement, yElement + 50);
                    SDL_RenderPresent(renderer);
                    
                    for (int tmp = 0; tmp < 3; tmp ++) {
                        if (checkIfDie(element[2], xElement+30, yElement, tree[treeNumber[tmp]], xTree[tmp], yGround + 15 - getTextureHeight(tree[treeNumber[tmp]]))) {
                            
                            if (ifPlayAgain()) {
                                //reset thong so ban dau
                                step = 5;
                                score = 0;
                                treeNumber[tmp] = rand() % 9;
                                xTree[tmp] = xTree[(tmp+2) % 3] + createRandomNumber(minDistance, maxDistance);
                            }
                            else running = false;
                        }
                    }
                    
                    changeParameters(xTree, treeNumber, groundPos, score, step);
                    
                    yTree[0] = yGround + 15 - getTextureHeight(tree[treeNumber[0]]);
                    yTree[1] = yGround + 15 - getTextureHeight(tree[treeNumber[1]]);
                    yTree[2] = yGround + 15 - getTextureHeight(tree[treeNumber[2]]);
                }

            }

        }
    }
    
    quitSDL(window, renderer);
    return 0;
}

void logSDLError(std::ostream& os, const std::string &msg, bool fatal) {
    os << msg << " Error: " << SDL_GetError() << std::endl;
    if (fatal) {
        SDL_Quit();
        exit(1);
    }
}

void initSDL(SDL_Window* &window, SDL_Renderer* &renderer) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        logSDLError(std::cout, "SDL_Init", true);
    //window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP);
    
    if (window == nullptr)
        logSDLError(std::cout, "CreateWindow", true);
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    //SDL_Renderer *renderer = SDL_CreateSoftwareRenderer(SDL_GetWindowSurface(window));
    if (renderer == nullptr)
        logSDLError(std::cout, "CreateRenderer", true);
    
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void quitSDL(SDL_Window* window, SDL_Renderer* renderer) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void waitUntilESC() {
    SDL_Event e;
    while (true) {
        if ( SDL_WaitEvent(&e) != 0 ) {
            if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE))
                return;
        }
        SDL_Delay(100);
    }
}


SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren) {
    SDL_Texture *texture = nullptr;
    SDL_Surface *loadedImage = SDL_LoadBMP(file.c_str());
    if (loadedImage != nullptr){
        texture = SDL_CreateTextureFromSurface(ren, loadedImage);
        SDL_FreeSurface(loadedImage);
        if (texture == nullptr){
            logSDLError(std::cout, "CreateTextureFromSurface");
        }
    }
    else {
        logSDLError(std::cout, "LoadBMP");
    }
    return texture;
}

// Vẽ ảnh giữ nguyên kích thước gốc
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y) {
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
    SDL_RenderCopy(ren, tex, NULL, &dst);
}

// Vẽ ảnh với kích thước tuỳ ý
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h) {
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(ren, tex, NULL, &dst);
}

void changeParameters(int xTree[], int treeNumber[], int &groundPos, long &score, int &step) {
    // Thay đổi các thông số, vị trí sau mỗi frame
    
    xTree[0] -= step;
    xTree[1] -= step;
    xTree[2] -= step;
    groundPos -= step;
    
    step = score / 200 + 5;
    minDistance = 400 + score/20;
    maxDistance = 900 + score/20;
    
    score++;
    
    
    if (xTree[0] <= 0) {
        treeNumber[0] = rand() % 9;
        xTree[0] = xTree[2] + createRandomNumber(minDistance, maxDistance);
    }
    if (xTree[1] <= 0) {
        treeNumber[1] = rand() % 9;
        xTree[1] = xTree[0] + createRandomNumber(minDistance, maxDistance);
    }
    if (xTree[2] <= 0) {
        treeNumber[2] = rand() % 9;
        xTree[2] = xTree[1] + createRandomNumber(minDistance, maxDistance);
    }
    
    if (groundPos + SCREEN_WIDTH <= 0) {
        groundPos = 0;
    }
    
    if (groundPos + SCREEN_WIDTH <= 0) {
        groundPos = 0;
    }
}

int getTextureHeight (SDL_Texture* tree) {
    int w, h;
    SDL_QueryTexture(tree, NULL, NULL, &w, &h);
    return h;
}

int getTextureWidth(SDL_Texture* tree) {
    int w, h;
    SDL_QueryTexture(tree, NULL, NULL, &w, &h);
    return w;
}

bool checkIfDie(SDL_Texture* element, int xElement, int yElement, SDL_Texture* tree, int xTree, int yTree) {
    int elementWidth = getTextureWidth(element) - 5, elementHeight = getTextureHeight(element) - 10, treeWidth = getTextureWidth(tree) - 8;
    if (xElement + elementWidth >= xTree && xElement <= xTree + treeWidth) {
        if (yElement + elementHeight >= yTree) return true;
    }
    return false;
}

bool ifPlayAgain() {
    SDL_Event e = waitPressedKey();
    if (e.key.keysym.sym == SDLK_SPACE) return true;
    return false;
}

void printScore(SDL_Renderer* renderer, long &score) {
    SDL_Surface *scoreSurface = NULL, *highScoreSurface = NULL;
    SDL_Texture *scoreTexture = NULL, *highScoreTexture = NULL;
    
    if (score > highScore) highScore = score;
    
    string scoreText = "Current score: " + to_string(score/10);
    string highScoreText = "High score: " + to_string(highScore/10);
    
    SDL_Color fg = {0, 0, 0, 0};
    TTF_Font* font = NULL;
    font = TTF_OpenFont("/Library/Fonts/AdobeArabic-Bold.otf", 40);
    
    scoreSurface = TTF_RenderText_Solid(font, scoreText.c_str(), fg);
    scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
    highScoreSurface = TTF_RenderText_Solid(font, highScoreText.c_str(), fg);
    highScoreTexture = SDL_CreateTextureFromSurface(renderer, highScoreSurface);
    
    SDL_Rect srcRest;
    SDL_Rect desRect;
    TTF_SizeText(font, scoreText.c_str(), &srcRest.w, &srcRest.h);
    
    srcRest.x = 0;
    srcRest.y = 0;
    
    desRect.x = 200;
    desRect.y = 270;
    
    desRect.w = srcRest.w;
    desRect.h = srcRest.h;
    
    renderTexture(scoreTexture, renderer, 100, SCREEN_HEIGHT / 10);
    renderTexture(highScoreTexture, renderer, SCREEN_WIDTH - getTextureWidth(highScoreTexture) - 100, SCREEN_HEIGHT / 10);
}
