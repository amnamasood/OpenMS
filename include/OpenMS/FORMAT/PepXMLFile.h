// -*- mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// --------------------------------------------------------------------------
//                   OpenMS Mass Spectrometry Framework
// --------------------------------------------------------------------------
//  Copyright (C) 2003-2009 -- Oliver Kohlbacher, Knut Reinert
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
// 
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// --------------------------------------------------------------------------
// $Maintainer: Chris Bielow, Hendrik Weisser $
// $Authors: Chris Bielow, Hendrik Weisser $
// --------------------------------------------------------------------------

#ifndef OPENMS_FORMAT_PEPXMLFILE_H
#define OPENMS_FORMAT_PEPXMLFILE_H

#include <OpenMS/METADATA/ProteinIdentification.h>
#include <OpenMS/METADATA/PeptideIdentification.h>
#include <OpenMS/FORMAT/HANDLERS/XMLHandler.h>
#include <OpenMS/FORMAT/XMLFile.h>
#include <OpenMS/CHEMISTRY/AASequence.h>
#include <OpenMS/KERNEL/MSExperiment.h>

#include <vector>
#include <map>
#include <set>


namespace OpenMS 
{
  /**
    @brief Used to load and store PepXML files
    
    This class is used to load and store documents that implement the schema of PepXML files.
		
		@todo Write tests (Chris, Hendrik)
		
  	@ingroup FileIO
  */
  class OPENMS_DLLAPI PepXMLFile
  	: protected Internal::XMLHandler,
  		public Internal::XMLFile
  {
		public:
		
			/// Constructor
			PepXMLFile();

			/// Destructor
			~PepXMLFile();
			
			/**
				@brief Loads peptide sequences with modifications out of a PepXML file
				
				@param filename PepXML file to load
				@param peptides Peptide identification output
				@param experiment MS run to extract the retention times from (PepXML contains only scan numbers). If the experiment is empty, it is read from @a experiment_name.
				@param experiment_name Experiment file name, which is used to extract the corresponding search results from the PepXML file. 
				
				@exception Exception::FileNotFound is thrown if the file could not be opened
				@exception Exception::ParseError is thrown if an error occurs during parsing
			*/
			void load(const String& filename, ProteinIdentification& protein, std::vector<PeptideIdentification>& peptides, MSExperiment<>& experiment, const String& experiment_name = "");
									
			/**
				@brief Stores idXML as PepXML file

				@exception Exception::UnableToCreateFile is thrown if the file could not be opened for writing
			*/
			void store(const String& filename, std::vector<ProteinIdentification>& protein_ids, std::vector<PeptideIdentification>& peptide_ids);

  	protected:
		
			// Docu in base class
			virtual void endElement(const XMLCh* const /*uri*/, const XMLCh* const /*local_name*/, const XMLCh* const qname);
			
			// Docu in base class
			virtual void startElement(const XMLCh* const /*uri*/, const XMLCh* const /*local_name*/, const XMLCh* const qname, const xercesc::Attributes& attributes);

		private:
			
		  void matchModification_(DoubleReal mass, String& modification_description);
		
			/// The sequence of the actual peptide hit				
			String actual_sequence_;
			
			/// The modifications of the actual peptide hit (position is 1-based)
			std::vector<std::pair<String, UInt> > actual_modifications_;
			
			/// stores the fixed residue modifications 
			std::vector<String> fixed_modifications_;

			/// stores the variable residue modifications
			std::vector<std::pair<String, DoubleReal> > variable_modifications_;

			/// Pointer to the ProteinIdentification
			ProteinIdentification* protein_;
						
			/// Pointer to the identified peptides
			std::vector<PeptideIdentification>* peptides_;
			
			/// Pointer to the experiment from which the pepXML file was generated
			const MSExperiment<>* experiment_;

			/// Name of the associated experiment (filename of the data file, extension will be removed)
			String exp_name_;	

			/// Pointer to the mapping between scan number in the pepXML file and index in the corresponding MSExperiment
			std::map<Size, Size>* scan_map_;

			/// Mass of a hydrogen atom (monoisotopic/average depending on case)
			DoubleReal hydrogen_mass_;

			/// Identifier linking PeptideIdentifications and ProteinIdentifications
			String prot_id_;

			/// Date the pepXML file was generated
			DateTime date_;

			/// Pointer to PeptideHit instance currently being processed
			PeptideHit* current_hit_;	

			/// Iterator to PeptideIdentification instance currently being processed
			std::vector<PeptideIdentification>::iterator current_pep_;		

			/// Are current entries belonging to the experiment of interest (for pepXML files that bundle results from different experiments)?
			bool wrong_experiment_;

			/// Search parameters
			ProteinIdentification::SearchParameters params_;

			/// Precursor ion charge
			Int charge_;

			/// Set of protein accessions (used to generate ProteinHits)
			std::set<String> accessions_;
						
			/// Retention time and mass-to-charge tolerance
			DoubleReal rt_tol_, mz_tol_;
			
			//@}
									
	};
 
} // namespace OpenMS

#endif // OPENMS_FORMAT_PEPXMLFILE_H
