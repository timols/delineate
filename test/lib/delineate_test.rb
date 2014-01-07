require 'test_helper'

class TestModule < Test::Unit::TestCase

  def test_version
    assert_equal Delineate::VERSION, '0.1.0'
  end

end
