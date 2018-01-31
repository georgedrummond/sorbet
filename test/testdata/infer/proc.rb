# @typed
class TestProcType
  sig(
    blk: T::Proc[Integer, returns: String],
  ).returns(String)
  def f(&blk)
    T.assert_type!(blk.call(7), String)
  end
end
