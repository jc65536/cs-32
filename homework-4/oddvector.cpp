void removeOdd(vector<int> &v) {
    auto it = v.begin();
    while (it != v.end()) {
        if (*it % 2)
            it = v.erase(it);
        else
            it++;
    }
}
