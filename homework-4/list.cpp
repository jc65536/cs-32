void listAll(string path, const Class *c) // two-parameter overload
{
    path += c->name();
    cout << path << endl;
    path += "=>";
    for (Class *cc : c->subclasses())
        listAll(path, cc);
}
