module Delineate
  class Line
    class << self; attr_accessor :tolerance; end
    @tolerance = 4 # degrees

    def ==(other_line)
      return (
        (self.start_point == other_line.start_point) &&
        (self.end_point   == other_line.end_point) &&
        (self.width       == other_line.width)
      )
    end

    def orientation
      angle_degrees = angle(true)

      return :vertical   if angle_degrees <= Line.tolerance + 90 and angle_degrees >= 90 - Line.tolerance
      return :horizontal if angle_degrees <= Line.tolerance      and angle_degrees >=  0 - Line.tolerance
      nil
    end

    def gradient
      rise.to_f / run.to_f
    end

    def rise
      (self.end_point.y - self.start_point.y).abs
    end

    def run
      (self.end_point.x - self.start_point.x).abs
    end

    def angle(degrees=false)
      radians = Math::atan(gradient)
      return radians * (180 / Math::PI) if degrees
      radians
    end
  end
end
