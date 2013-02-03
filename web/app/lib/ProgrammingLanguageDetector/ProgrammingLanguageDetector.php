<?php
	/*
	 * Programming Language Detector - https://github.com/bakwc/Pastexen
	 * Copyright (C) 2013 powder96 <https://github.com/powder96>
	 *
	 * ProgrammingLanguageDetector::cleanSource function is based
	 * on the source code from <https://github.com/github/linguist/>
	 * Copyright (c) 2011 GitHub, Inc.
	 *
	 * Permission is hereby granted, free of charge, to any person
	 * obtaining a copy of this software and associated documentation
	 * files (the "Software"), to deal in the Software without
	 * restriction, including without limitation the rights to use,
	 * copy, modify, merge, publish, distribute, sublicense, and/or sell
	 * copies of the Software, and to permit persons to whom the
	 * Software is furnished to do so, subject to the following
	 * conditions:
	 * 
	 * The above copyright notice and this permission notice shall be
	 * included in all copies or substantial portions of the Software.
	 * 
	 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
	 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
	 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
	 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
	 * OTHER DEALINGS IN THE SOFTWARE.
	 */
	
	final class ProgrammingLanguageDetector {
		private $knowledgeBase = array();
		
		public function detect($source) {
			$languagesScore = $this->score($source);
			$bestLanguage = 'undefined';
			$bestLanguageScore = 0;
			foreach($languagesScore as $language => $score)
				if($score > $bestLanguageScore) {
					$bestLanguage = $language;
					$bestLanguageScore = $score;
				}
			
			return $bestLanguage;
		}
		
		public function exportKnowledgeBase() {
			return gzcompress(serialize($this->knowledgeBase));
		}
		
		public function importKnowledgeBase($data) {
			$this->knowledgeBase = unserialize(gzuncompress($data));
		}
		
		public function train($source, $language) {
			$sourceWords = self::countWordsFreqSource(self::cleanSource($source));
			
			if($sourceWords['total'] == 0)
				return;
			
			if(!isset($this->knowledgeBase[$language]['total']))
				$this->knowledgeBase[$language]['total'] = 0;
			$this->knowledgeBase[$language]['total'] += $sourceWords['total'];
			
			foreach($sourceWords['freq'] as $word => $frequency) {
				if(!isset($this->knowledgeBase[$language]['freq'][$word]))
					$this->knowledgeBase[$language]['freq'][$word] = 0;
				$this->knowledgeBase[$language]['freq'][$word] += $frequency;
			}
		}
		
		private function score($source) {
			$languagesScore = array();
			$sourceWords = self::countWordsFreqSource(self::cleanSource($source));
			foreach($this->knowledgeBase as $language => $kbWords) {
				$languagesScore[$language] = 0;
				
				foreach($kbWords['freq'] as $word => $kbFrequency) {
					if(!isset($sourceWords['freq'][$word]))
						continue;
					
					$kbFrequencyWeighted = $kbFrequency / $kbWords['total'];
					$srcFrequencyWeighted = $sourceWords['freq'][$word] / $sourceWords['total'];
				
					$languagesScore[$language] += 1 - abs($srcFrequencyWeighted - $kbFrequencyWeighted);
				}
			}
			
			return $languagesScore;
		}
		

		
		private static function cleanSource($source) {
			// strip single line comments
			$source = preg_replace('/(\/\/|#|%)(.*)/', '', $source);
			
			// strip multi line comments
			$source = preg_replace('/\/\*(.*)\*\//Us', '', $source); // C
			$source = preg_replace('/<!--(.*)-->/Us' , '', $source); // XML
			$source = preg_replace('/{-(.*)-}/Us'    , '', $source); // Haskell
			$source = preg_replace('/\(\*(.*)\*\)/Us', '', $source); // Coq
			
			// strip strings
			$source = preg_replace('/\'(.*)\'/Us', '', $source); // single quoted
			$source = preg_replace('/\"(.*)\"/Us', '', $source); // double quoted
			
			// strip numbers
			$source = preg_replace('/(0x)?\d(\d|\.)*/', '', $source);
			
			return $source;
		}
		
		private static function countWordsFreqSource($source) {
			$words = array_filter(preg_split('/[^A-Za-z]/', $source));
			$wordsTotal = count($words);
			
			$wordsFrequency = array();
			foreach($words as $word) {
				$word = strtolower($word);
				
				if(!isset($wordsFrequency[$word]))
					$wordsFrequency[$word] = 0;
				
				++$wordsFrequency[$word];
			}
			
			return array('total' => $wordsTotal, 'freq' => $wordsFrequency);
		}
	}