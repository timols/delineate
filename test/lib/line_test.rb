require 'test_helper'

class TestLine < Test::Unit::TestCase

  def setup
    @start_point = Delineate::Point.new(0.0, 0.0)
    @end_point   = Delineate::Point.new(10.0, 2.0)
    @line        = Delineate::Line.new(@start_point, @end_point, 2.0)
  end

  def test_line_new
    assert Delineate::Line.respond_to? :new, 'responds to new'
  end

  def test_line_start_point
    assert_equal @start_point, @line.start_point, 'get start point'
  end

  def test_line_end_point
    assert_equal @start_point, @line.start_point, 'get start point'
  end

  def test_line_width
    assert_equal 2.0, @line.width, 'get width'
  end

  def test_line_set_start_point
    new_start_point   = Delineate::Point.new(1.0, 2.0)
    @line.start_point = new_start_point
    assert @line.start_point == new_start_point, 'set start point'
  end

  def test_line_set_end_point
    new_end_point   = Delineate::Point.new(15.0, 2.0)
    @line.end_point = new_end_point
    assert @line.end_point == new_end_point, 'set end point'
  end

  def test_line_set_width
    @line.width = 4.0
    assert_equal 4.0, @line.width, 'set width'
  end

  def test_line_not_equal
    @other_line = Delineate::Line.new(Delineate::Point.new, Delineate::Point.new, 0.0)
    assert @line != @other_line, 'should be different'
  end

  def test_line_equal
    @other_line = Delineate::Line.new(@start_point, @end_point, 2.0)
    assert @line == @other_line, 'should be equal'
  end

  def test_line_no_orientation
    assert @line.orientation.nil?, 'should not be horizontal or vertical'
  end

  def test_line_vertical
    @line.start_point = Delineate::Point.new(10.0, 30.0)
    assert @line.orientation == :vertical, 'should be vertical'
  end

  def test_line_horizontal
    @line.start_point = Delineate::Point.new(0.0, 2.0)
    assert @line.orientation == :horizontal, 'should be horizontal'
  end

  def test_line_tolerance
    assert_equal 4, Delineate::Line.tolerance, 'get line tolerance'
  end

  def test_set_line_tolerance
    Delineate::Line.tolerance = 5
    assert_equal 5, Delineate::Line.tolerance, 'set line tolerance'
  end

  def test_line_gradient
    line = Delineate::Line.new(Delineate::Point.new(0, 0), Delineate::Point.new(1, 1), 1)
    assert_equal 1, line.gradient, 'should determine the gradient of the line'
  end

  def test_line_to_string
    assert_equal '<Delineate::Line: width=(2.0) start_point=[0.0, 0.0] end_point=[10.0, 2.0]>', @line.to_s, 'should have a string representation'
  end

  def test_line_to_array
    assert_equal [@start_point, @end_point, 2.0], @line.to_ary, 'should have an array representation'
  end

end
