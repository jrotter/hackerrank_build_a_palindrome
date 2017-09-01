# This test is essential :)
def is_palindrome?(a)
  a == a.reverse
end

# Read in the data
count = gets.strip.to_i
count.times do |i|
  s1 = gets.strip
  s2 = gets.strip

  best = nil
  # Run through all possible substrings of s1
  for x in (0..(s1.length-1))
    for y in (x..(s1.length-1))
      # s1[x..y] is our current substring of s1
      for a in (0..(s2.length-1))
        for b in (a..(s2.length-1))
          # s2[a..b] is our current substring of s2
          test_string = s1[x..y] + s2[a..b]
          if is_palindrome?(test_string)
            if best == nil  || 
               (test_string.length > best.length) ||
               ((test_string.length == best.length) && (test_string < best))
              best = test_string
            end
          end
        end
      end
    end
  end
  if best
    puts best
  else
    puts "-1"
  end
end

