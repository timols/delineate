require 'test_helper'

class TestPoint < Test::Unit::TestCase

  def setup
    @point = Delineate::Point.new
  end

  def test_point_new
    assert Delineate::Point.respond_to? :new, 'responds to new'
  end

  def test_point_x
    assert @point.x == 0.0, 'initializes with x co-ord of 0'
  end

  def test_point_y
    assert @point.y == 0.0, 'initializes with y co-ord of 0'
  end

  def test_point_x_set
    @point.x = 2.0
    assert @point.x == 2.0, 'set x co-ord'
  end

  def test_point_y_set
    @point.y = 2.0
    assert @point.y == 2.0, 'set y co-ord'
  end

  def test_point_to_string
    assert @point.to_s == '<Delineate::Point:(0,0)>', 'has a string representation'
  end

  def test_point_to_array
    assert @point.to_ary == [0.0, 0.0], 'has an array representation'
  end

  def test_point_equals
    @other_point = Delineate::Point.new
    assert @point == @other_point, 'equivalent to another point'
  end

  def test_point_compatibility
    @other_point = mock "SomeOtherPointClass"
    @other_point.stubs(:x).returns(1.0)
    @other_point.stubs(:y).returns(1.0)

    assert @point.class.compatible?(@other_point), 'is compatible with other point classes'
  end
end
