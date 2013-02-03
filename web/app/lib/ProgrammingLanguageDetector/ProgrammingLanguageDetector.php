<?php
	/*
	 * Pastexen web frontend - https://github.com/bakwc/Pastexen
	 * Copyright (C) 2013  powder96 <https://github.com/powder96>
	 *
	 * This program is free software: you can redistribute it and/or modify
	 * it under the terms of the GNU General Public License as published by
	 * the Free Software Foundation, either version 3 of the License, or
	 * (at your option) any later version.
	 *
	 * This program is distributed in the hope that it will be useful,
	 * but WITHOUT ANY WARRANTY; without even the implied warranty of
	 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	 * GNU General Public License for more details.
	 *
	 * You should have received a copy of the GNU General Public License
	 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
	 */
	
	final class ProgrammingLanguageDetector {
		private $knowledgeBase = array();
		
		public function detect($source) {
			$languagesScore = $this->score(self::removeComments($source));
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
			$sourceWords = self::countWords(self::removeComments($source));
			
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
			$sourceWords = self::countWords($source);
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
		
		private static function countWords($source) {
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
		
		// this method removes comments from the source
		// NOTE: if // and /* */ are not comments in your programming language, then this method will erase some of your code!
		private static function removeComments($source) {
			$source = preg_replace('/\/\/(.*)/', '', $source);
			$source = preg_replace('/\/\*(.*)\*\//Us', '', $source);
			
			return $source;
		}
	}