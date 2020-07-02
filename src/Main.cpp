#include <Siv3D.hpp> // OpenSiv3D v0.4.3

class GameBoard {
private:
    
    // 11x11 の二次元配列
    Grid<int32> m_grid = Grid<int32>(11, 11);
    
    // これから置くマーク
    int32 m_currentMark = O_Mark;
    
    // 格子を描く
    void drawGridLines() const {
        
        // 線を引く
        for (auto i : Range(1, 10)) {
            Line(i * CellSize, 0, i * CellSize, 11 * CellSize)
                .draw(4, ColorF(0.25));
            
            Line(0, i * CellSize, 11 * CellSize, i * CellSize)
                .draw(4, ColorF(0.25));
        }
    }
    
    // セルを描く
    void drawCells() const {
        
        // 11x11 のセル
        for (auto p : step(Size(11, 11))) {
            
            // セル
            const Rect cell(p * CellSize, CellSize);
            
            // セルのマーク
            const int32 mark = m_grid[p];
            
            if (mark == X_Mark) {
                // X マークを描く
                Shape2D::Cross(CellSize * 0.4, 10, cell.center())
                    .draw(ColorF(0.2));
                
                continue;
            }
            else if (mark == O_Mark) {
                // O マークを描く
                Circle(cell.center(), CellSize * 0.4 - 2)
                    .drawFrame(9, 0, ColorF(0.2));
                
                continue;
            }
            
            // セルがマウスオーバーされたら
            if (cell.mouseOver()) {
                
                // カーソルを手のアイコンに
                Cursor::RequestStyle(CursorStyle::Hand);
                
                // セルの上に半透明の白を描く
                cell.stretched(-2).draw(ColorF(1.0, 0.6));
            }
        }
    }
    
public:
    
    // セルの大きさ
    static constexpr int32 CellSize = 50;
    
    // O マークの値
    static constexpr int32 O_Mark = 1;
    
    // X マークの値
    static constexpr int32 X_Mark = 2;
    
    GameBoard() {
        /*
        // 表示テスト用
        m_grid[{0, 0}] = O_Mark;
        m_grid[{1, 0}] = X_Mark;
        */
    }
    
    void update() {
        
        // 11x11 のセル
        for (auto p : step(Size(11, 11))) {
            
            // セル
            const Rect cell(p * CellSize, CellSize);
            
            // セルのマーク
            const int32 mark = m_grid[p];
            
            // セルが空白で、なおかつクリックされたら
            if ((mark == 0) && cell.leftClicked()) {
                
                // セルにマークを書き込む
                m_grid[p] = m_currentMark;
                
                // 現在のマークを入れ替える
                m_currentMark = ((m_currentMark == O_Mark) ? X_Mark : O_Mark);
            }
        }
    }
    
    // 描画
    void draw() const {
        
        drawGridLines();
        
        drawCells();
    }
};

void Main(){
    
    // 背景色
    Scene::SetBackground(ColorF(0.8, 1.0, 0.9));
    
    constexpr Point offset(70, 30);
    
    GameBoard gameBoard;
    
    while (System::Update()) {
        // 2D 描画とマウスカーソル座標を移動
        Transformer2D tr(Mat3x2::Translate(offset), true);
        
        gameBoard.update();
        
        gameBoard.draw();
    }
}

