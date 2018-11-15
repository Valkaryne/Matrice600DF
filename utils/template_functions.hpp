#ifndef TEMPLATE_FUNCTIONS_HPP
#define TEMPLATE_FUNCTIONS_HPP

template<typename Base, typename T>
inline bool instanceof(const T *ptr) {
    return dynamic_cast<const Base*>(ptr) != nullptr;
}

#endif // TEMPLATE_FUNCTIONS_HPP
