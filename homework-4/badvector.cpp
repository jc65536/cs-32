void removeBad(vector<Movie *> &v) {
    auto it = v.begin();
    while (it != v.end()) {
        Movie *mov = *it;
        if (mov->rating() < 50) {
            delete mov;
            it = v.erase(it);
        } else {
            it++;
        }
    }
}
