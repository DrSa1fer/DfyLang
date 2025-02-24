space std.io {
    func clear { extern ("cs", "System.Console.Clear();") }

    func input  > text { extern ("cs", "System.Console.Read();") }
    func inputl > text { extern ("cs", "System.Console.ReadLine();") }

    func print  (t : text) { extern ("cs", "System.Console.Write($t);") }
    func printl (t : text) { extern ("cs", "System.Console.WriteLine($t);") }
}