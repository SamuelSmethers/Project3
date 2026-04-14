// ============================================================================
// Hash PIN
unsigned long hashPin(const string& pin) {
    unsigned long h = 0;
    for (int i = 0; i < (int)pin.size(); i++) {
        h = h * 31 + pin.at(i);
    }
    return h;
}
