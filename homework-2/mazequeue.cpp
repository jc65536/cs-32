#include <queue>
#include <string>
#include <iostream>

class Coord {
public:
    Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
    int r() const { return m_r; }
    int c() const { return m_c; }

private:
    int m_r;
    int m_c;
};

const int dr[] = {0, 1, 0, -1};
const int dc[] = {-1, 0, 1, 0};
const char VISITED = 'v';

bool pathExists(std::string maze[], int nRows, int nCols, int sr, int sc, int er, int ec) {
    /*
    Push the starting coordinate (sr,sc) onto the coordinate stack and
        update maze[sr][sc] to indicate that the algorithm has encountered
        it (i.e., set maze[sr][sc] to have a value other than '.').
    While the stack is not empty,
    {   Pop the top coordinate off the stack. This gives you the current
            (r,c) location that your algorithm is exploring.
        If the current (r,c) coordinate is equal to the ending coordinate,
            then we've solved the maze so return true!
        Check each place you can move from the current cell as follows:
            If you can move WEST and haven't encountered that cell yet,
                then push the coordinate (r,c-1) onto the stack and update
                maze[r][c-1] to indicate the algorithm has encountered it.
            If you can move SOUTH and haven't encountered that cell yet,
                then push the coordinate (r+1,c) onto the stack and update
                maze[r+1][c] to indicate the algorithm has encountered it.
            If you can move EAST and haven't encountered that cell yet,
                then push the coordinate (r,c+1) onto the stack and update
                maze[r][c+1] to indicate the algorithm has encountered it.
            If you can move NORTH and haven't encountered that cell yet,
                then push the coordinate (r-1,c) onto the stack and update
                maze[r-1][c] to indicate the algorithm has encountered it.
    }
    There was no solution, so return false
    */
    std::queue<Coord> q;
    q.push(Coord(sr, sc));
    maze[sr][sc] = VISITED;
    while (!q.empty()) {
        Coord current = q.front();
        q.pop();
        if (current.r() == er && current.c() == ec)
            return true;
        for (int i = 0; i < 4; i++) {
            const int r = current.r() + dr[i], c = current.c() + dc[i];
            if (r >= 0 && r < nRows && c >= 0 && c < nCols && maze[r][c] == '.') {
                q.push(Coord(r, c));
                maze[r][c] = VISITED;
            }
        }
    }
    return false;
}

int main() {
    std::string maze[10] = {
        "XXXXXXXXXX",
        "X.X..X...X",
        "X....XXX.X",
        "X.XXXX.X.X",
        "X......XXX",
        "X.XX.X...X",
        "X.X..X.X.X",
        "X.X.XXXX.X",
        "X.X...X..X",
        "XXXXXXXXXX"};

    if (pathExists(maze, 10, 10, 4, 6, 1, 1))
        std::cout << "Solvable!" << std::endl;
    else
        std::cout << "Out of luck!" << std::endl;
}
