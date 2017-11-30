# @typed
class T1; end
class T2; end

class A
  standard_method(
    {
      a: [T1, T2],
      b: T1,
      c: Opus::Types.nilable(T1),
      d: Opus::Types.any(T1, T2),
      e: Opus::Types.untyped,
      f: Opus::Types.array_of(T1),
      g: Opus::Types.hash_of(keys: T1, values: T2),
      h: Opus::Types.enum([false, 1, 3.14, "foo", :bar]),
    }, returns: T2)
  def good(a, b, c, d, e, f, g, h)
  end

  standard_method(
    {
      a: unsupported, # error: Unknown type syntax
      b: Opus::Types.enum, # error: enum only takes a single argument
      c: Opus::Types.enum(1), # error: enum must be passed a literal array
      d: Opus::Types.enum([]), # error: enum([]) is invalid
      e: Opus::Types.enum([meth]), # error: Unsupported type literal
    }, returns: T2)
  def bad(a, b, c, d, e)
  end

  standard_method({}, returns: Opus::Types.noreturn)
  def noreturn
  end

  standard_method(returns: T1)
  def no_params
  end

  standard_method({returns: T1}, returns: T2, checked: false)
  def test_kwargs(returns)
  end

  standard_method(types, returns: T1) # error: Expected a hash literal
  def f1(x)
  end

  standard_method({x: T1}, "returns" => T1) # error: Keys must be symbol literals
  def f2(x)
  end

  standard_method({x: T1}, returns: T1)
  private def private(x)
      T1.new
  end

  standard_method({x: T1}, returns: T1)
  protected def protected(x)
      T1.new
  end

  standard_method({x: T1}, returns: T1)
  public def public(x)
      T1.new
  end

  standard_method({x: T1}, returns: T1)
  private_class_method def self.static(x)
      T1.new
  end

  standard_method({y: T1}, returns: T1) # error: Unused standard_method. No method def before next standard_method.
  standard_method({y: T1}, returns: T1)
  def f3(y)
  end

  standard_method({z: T1}, returns: T1) # error: Malformed standard_method. No method def following it.
end
