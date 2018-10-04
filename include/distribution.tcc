/**
 * @contructor Constructor with multiple arguments
 */
template <typename... Targs>
Distribution::Distribution(Targs... args) : model_name_{"Abstract"} {}
