void removeBad(list<Movie *> &li) {
    auto it = li.begin();
    while (it != li.end()) {
        Movie *mov = *it;
        if (mov->rating() < 50) {
            delete mov;
            it = li.erase(it);
        } else {
            it++;
        }
    }
}
