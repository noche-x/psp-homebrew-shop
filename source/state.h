#pragma once

class state {
public:
    virtual void init() = 0;
    virtual void destroy() = 0;
    virtual void update() = 0;
    virtual void draw() = 0;
	virtual bool should_change() = 0;
};
