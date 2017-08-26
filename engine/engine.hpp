#ifndef ENGINE_HPP_
#define ENGINE_HPP_

class engine {
public:
    engine();
    engine(engine const &) = delete;
    ~engine();
    void operator=(engine const &) = delete;
    void loop();
};

#endif // ENGINE_HPP_
