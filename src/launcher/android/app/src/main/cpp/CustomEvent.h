#ifndef DELAUNCHER_CUSTOMEVENT_H
#define DELAUNCHER_CUSTOMEVENT_H

class CustomEvent {
public:
    enum class Type{
        backButton
    };

    Type type = Type::backButton;

    CustomEvent() = default;
};

#endif //DELAUNCHER_CUSTOMEVENT_H
