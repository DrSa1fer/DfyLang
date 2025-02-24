space std.math {
    func pow (n : number, power : number) > number { return for i in range(2, power) { n * n } }
}