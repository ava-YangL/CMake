/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile$
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

Copyright (c) 2001 Insight Consortium
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

 * The name of the Insight Consortium, nor the names of any consortium members,
   nor of any contributors, may be used to endorse or promote products derived
   from this software without specific prior written permission.

  * Modified source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/
#include "cmBuildCommand.h"

// cmBuildCommand
bool cmBuildCommand::Invoke(std::vector<std::string>& args)
{
  if(args.size() < 2 )
    {
    this->SetError("called with incorrect number of arguments");
    return false;
    }
  const char* define = args[0].c_str();
  const char* cacheValue
    = cmCacheManager::GetInstance()->GetCacheValue(define);
  if(cacheValue)
    {
    return true;
    }
  std::string makecommand;
  std::string& makeprogram = args[1];
  m_Makefile->ExpandVariablesInString(makeprogram);
  if(makeprogram.find("msdev") != std::string::npos ||
     makeprogram.find("MSDEV") != std::string::npos )
    {
    makecommand = "\"";
    makecommand += makeprogram;
    makecommand += "\"";
    makecommand += " ";
    makecommand += m_Makefile->GetProjectName();
    makecommand += ".dsw /MAKE \"ALL_BUILD - Debug\" /REBUILD";
    }
  else
    {
    makecommand = makeprogram;
    makecommand += " -k";
    }
  cmCacheManager::GetInstance()->
    AddCacheEntry(define,
                  makecommand.c_str(),
                  "Command used to build entire project from the command line.",
                  cmCacheManager::STRING);
  m_Makefile->AddDefinition(define, makecommand.c_str());
  return true;
}

