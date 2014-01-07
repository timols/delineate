require 'test_helper'

class TestDetection < Test::Unit::TestCase

  def setup
    @horizontal = File.expand_path('../../fixtures/basic_horizontal.png', __FILE__)
    @vertical   = File.expand_path('../../fixtures/basic_vertical.png',   __FILE__)
    @horizontal_vertical = File.expand_path('../../fixtures/basic_horizontal_vertical.png',   __FILE__)
  end

  def test_open_file
    assert Delineate.open_image(@horizontal), 'should open file'
    Delineate.close_image
  end

  def test_horizontal_detection_with_horizontal_image
    Delineate.open_image(@horizontal)
    lines = Delineate.horizontal_lines(30, 1)
    assert_equal 1, lines.length, 'should detect a horizontal line'

    lines = Delineate.vertical_lines(30, 1)
    assert_equal 0, lines.length, 'should not detect a vertical line'
    Delineate.close_image
  end

  def test_vertical_detection_with_vertical_image
    Delineate.open_image(@vertical)
    assert_equal 0, Delineate.horizontal_lines(30, 1).length, 'should not detect a horizontal line'

    lines = Delineate.vertical_lines(30, 1)
    assert_equal 1, lines.length, 'should detect a vertical line'
    Delineate.close_image
  end

  def test_horizontal_and_vertical_detection
    Delineate.open_image(@horizontal_vertical)
    assert_equal 1, Delineate.horizontal_lines(30, 1).length, 'should detect a horizontal line'
    lines = Delineate.vertical_lines(30, 1)
    puts lines.map(&:to_s).join("\n")
    assert_equal 1, lines.length, 'should detect a vertical line'
    Delineate.close_image
  end

  def test_close_file
    Delineate.open_image(@horizontal)
    assert Delineate.close_image, 'should close file'
  end


end
