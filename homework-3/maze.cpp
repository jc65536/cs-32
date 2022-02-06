bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec) {
    const int dr[] = {-1, 0, 1, 0};
    const int dc[] = {0, 1, 0, -1};
    if (sr == er && sc == ec)
        return true;
    maze[sr][sc] = 'v';
    for (int i = 0; i < 4; i++) {
        const int r = sr + dr[i], c = sc + dc[i];
        if (r >= 0 && r < nRows && c >= 0 && c < nCols && maze[r][c] == '.' &&
            pathExists(maze, nRows, nCols, r, c, er, ec)) {
            return true;
        }
    }
    return false;
}
