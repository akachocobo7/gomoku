#include <Siv3D.hpp> // OpenSiv3D v0.4.3


// マークがつながったかを返す関数
bool checkLine(const Grid<int32>& grid, const Array<Point>& cells){
    
    if (cells.empty() || grid[cells[0]] == 0) return false;
    
    for (auto cell : cells) {
        if (grid[cells[0]] != grid[cell]) {
            return false;
        }
    }
    
    return true;
}

// マークがつながったラインの一覧を返す関数
Array<std::pair<Point, Point>> checkLines(const Grid<int32>& grid) {
    
    Array<std::pair<Point, Point>> results;
    
    // 右上, 右, 右下, 下
    const int32 dy[4] = {-1, 0, 1, 1};
    const int32 dx[4] = {1, 1, 1, 0};
    
    for (auto p : step(Size(11, 11))) {
        for (auto i : step(4)) {
            Array<Point> cells;
            auto [y, x] = p;
            
            for (auto _ : step(5)) {
                if (y < 0 || y >= 11 || x < 0 || x >= 11) {
                    break;
                }
                
                cells.push_back({y, x});
                y += dy[i];
                x += dx[i];
            }
            
            if (cells.size() == 5 && checkLine(grid, cells)) {
                results.push_back({cells[0], cells[4]});
            }
        }
    }
    
    return results;
}

class GameBoard {
private:
    
    // 11x11 の二次元配列
    Grid<int32> m_grid = Grid<int32>(11, 11);
    
    // これから置くマーク
    int32 m_currentMark = O_Mark;
    
    // ゲーム終了フラグ
    bool m_gameOver = false;
    
    // 5つ連続したラインの一覧
    Array<std::pair<Point, Point>> m_lines;
    
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
            if (!m_gameOver && cell.mouseOver()) {
                
                // カーソルを手のアイコンに
                Cursor::RequestStyle(CursorStyle::Hand);
                
                // セルの上に半透明の白を描く
                cell.stretched(-2).draw(ColorF(1.0, 0.6));
            }
        }
    }
    
    // つながったラインを描く
    void drawResults() const {
        
        for (const auto& line : m_lines){
            
            // つながったラインの始点と終点のセルを取得
            const Rect cellBegin(line.first * CellSize, CellSize);
            const Rect cellEnd(line.second * CellSize, CellSize);
            
            // 線を引く
            Line(cellBegin.center(), cellEnd.center())
                .stretched(CellSize * 0.45)
                .draw(LineStyle::RoundCap, 5, ColorF(0.6));
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
        
        if (m_gameOver) {
            return;
        }
        
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
                
                // つながったラインを探す
                m_lines = checkLines(m_grid);
                
                // 空白セルが 0 になるか、つながったラインが見つかったら
                if (m_grid.count(0) == 0 || m_lines) {
                    
                    // ゲーム終了
                    m_gameOver = true;
                }
            }
        }
    }
    
    // 描画
    void draw() const {
        
        drawGridLines();
        
        drawCells();
        
        drawResults();
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

