require 'digest/sha1'
require 'benchmark'

class Array
  def shuffle!
    size.downto(1) { |n| push delete_at(rand(n)) }
    self
  end
end


WORDCHARS = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ".split(//)
DICTIONARY = %w{ Cloud Ruby DHH one eight six active record controller data rspec mongrel MySQL postgresSQL tokyo MRI jruby rubinius memcached exception metaprogramming reflection }
PHRASE = "I am not a big believer in fortune telling"

PHRASE_SHA1 = Digest::SHA1.hexdigest(PHRASE)
PHRASE_BINARY = Integer("0b#{PHRASE_SHA1.to_i(base=16).to_s(2)}")

$best_distance = 10000
$best_content = ""

# 10 = 1
# 01 = 1
# 11 = 0
# 00 = 0

def calculate(phrase)
  sha1 = Digest::SHA1.hexdigest(phrase)
  distance = (Integer("0b#{sha1.to_i(base=16).to_s(2)}") ^ PHRASE_BINARY).to_s(2).gsub(/0/, '').length

  return unless distance < $best_distance
  $best_distance = distance
  $best_content = phrase
  puts "#{distance} #{phrase}"
end

def calculate_with_letters(phrase, letters)
  return if letters.length >= 5
  WORDCHARS.each {|l| calculate(phrase + ' ' + letters + l); calculate_with_letters(phrase, letters + l) }
end

def naive
  1.upto(10) do
    phrase = DICTIONARY.shuffle![0,12].join(' ')
    calculate(phrase)
    calculate_with_letters(phrase, "")
  end
end

Benchmark.bm do |x|
  x.report("naive") { naive }
end
