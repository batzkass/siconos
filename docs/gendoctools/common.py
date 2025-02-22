"""Common functions used in all steps of Siconos
documentation generation process.


Siconos is a program dedicated to modeling, simulation and control
 of non smooth dynamical systems.

 Copyright 2022 INRIA.

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
"""
import shutil
import pickle
import textwrap
from pathlib import Path

try:
    import lxml.etree as ET
except ImportError:
    import xml.etree.ElementTree as ET


def get_xml_files(header_name, xml_path, case_sense_names=True):
    """Return all xml files generated by doxygen from a given header file.
    and found in a given path.

    Parameters
    ----------
    header_name : Path()
        header file.
    xml_path : Path()
        absolute path to xml files.
    case_sense_names : bool
        false if xml output files names are lower case only

    Returns a list of xml files (Path()).

    Notes
    -----

    * This function takes into account the value of CASE_SENSE_NAMES parameter
    in doxygen config.
    * It looks for '*classNAME.xml', '*structNAME.xml' and 'NAME_8h*.xml'
    name being the input header name (without ext).

    """
    # Get filename without extension
    fnwe = header_name.stem
    # Replace _ with __ in filename
    fnwe = fnwe.replace('_', '__')
    if not case_sense_names:
        fnwe = replace_uppercase_letters(fnwe)
    # Look for 'class' files
    classfiles = [f for f in xml_path.glob('class' + fnwe + '.xml')]
    # Look for 'struct' files
    structfiles = [f for f in xml_path.glob('struct' + fnwe + '.xml')]
    # Look for '8h' (?) files
    files8h = [f for f in xml_path.glob(fnwe + '_8h*.xml')]
    allfiles = classfiles + structfiles + files8h
    return allfiles


def parse_doxygen_config(filename):
    """Read doxygen config into a python dictionnary

    Parameters
    ----------
    filename : string or Path()
       full path to doxygen file

    Returns a python dictionnary
    """
    result = {}
    with open(filename) as ff:
        # remove comment lines
        conf = [n.strip() for n in ff.readlines()
                if (not n.startswith('#') and not n.startswith('\n'))]
        for d in conf:
            if d.count('=') > 0:
                res = d.split('=')
                result[res[0].rsplit()[0]] = res[1]
                backup = res.copy()
            else:
                result[backup[0].rsplit()[0]] += d
    return result


def filter_dot_in_xml_formulas(xmlfile):
    """Replace \\dot with \dot in xml input.

    dot is confusing for doxygen (may be latex or graphviz),
    so in latex formula inside \rst 
    we need to use \\dot and replace it later, 
    when breathe/sphinx comes into action.

    Parameters
    ----------
    xmlfile: Path()
        xml file name (full path) (in-out param)
    """
    fileout = xmlfile.stem + '.tmp'
    with open(xmlfile, 'r+', encoding='utf8') as f:
        lines = f.read()
    newlines = lines.replace(r'\\dot', r'\dot')
    with open(fileout, 'w', encoding='utf8') as f:
        f.write(newlines)
    shutil.move(fileout, xmlfile)


def replace_uppercase_letters(filename):
    """Replace uppercase letters in a string
    with _lowercase (following doxygen way)

    e.g. : TimeStepping --> _time_stepping

    This is useful to postprocess filenames from xml-doxygen outputs
    and feed them to doxy2swig, even when CASE_SENSE_NAMES = NO
    in doxygen config.

    Usage:

    result = replace_uppercase_letters(input)
    """
    r = []
    for c in filename:
        # l being: last character was not uppercase
        newc = c
        if c.isupper():
            newc = '_' + c.lower()
        r.append(newc)
    return ''.join(r)


def get_xml_compound_infos(compound):
    """Returns object (class, struct ...) name, kind
    and brief description
    from an xml node (compound)
    """
    kind = compound.attrib['kind']
    names = compound.findall('compoundname')
    descr = compound.findall('briefdescription')
    assert len(names) == 1
    assert len(descr) == 1
    #res = ''
    content = descr[0].find('para')
    if content is not None:
        res = ET.tostring(content, method='text')
        # for child in descr[0]:
        #     try:
        #         res += child.text
        #     except:
        #         pass
        #     for sub in child:
        #         try :
        #             res += sub.text
        #         except:
        #             pass
        # new lines in description must be indented
        res = '\n    '.join(res.decode().split('\n'))
        res = find_and_replace_math(res)
    else:
        res = ''
    return names[0].text, kind, res


def find_and_replace_math(content):
    """Replace latex-like math formulas in content
    with sphinx-like math formulas

    content : a string.
    """
    if content.count('$') > 1:
        start = content.find('$')
        end = content.find('$', start + 1)
        doxy_latex = content[start: end + 1]
        rst_latex = r':math:`' + doxy_latex[1:-1].strip() + r'`'
        content = content.replace(doxy_latex, rst_latex)
    return content


def replace_latex(inoutfile, latex_sources):
    """Post processing of latex forms in docstrings.

    Parameters
    ----------

    inoutfile : Path()
       name (full path) of the python file to process
    latex_sources : string or Path()
       directory which contains pickle files with latex forms
       (result of do_latex or do_verbatim call in sicodoxy2swig)

    Usually : inoutfile = some_component.py (e.g. numerics.py)
    and latex_dir = wrap/siconos/tmp_component_name.

    This function is supposed to be called by a target
    generated with cmake (make <component>_replace_latex)

    It is also called  by module_docstrings2rst.
    """
    latex_sources = Path(latex_sources).resolve()

    # Parse latex_sources and get all pickle files
    formfiles = [f for f in latex_sources.glob('latex_*.pickle')]

    # Runner : use sed rather than python replace. But less portable (?)
    #runner = os.path.join(os.path.dirname(os.path.abspath(__file__)),
    #                      'replace_latex.sh')

    # temp file for outputs.
    inoutfile = Path(inoutfile)
    target = Path(inoutfile.parent, inoutfile.stem + '.copy').resolve()
    shutil.copyfile(inoutfile, target)
    # Read input (.py)
    with open(inoutfile, "r") as f:
        source_lines = f.readlines()
    rst = []
    # Parse and replace :
    # for each formula found in each pickle file,
    # replace FORMULA_Id with the proper string
    # in temp list.
    for fname in formfiles:
        with open(fname, 'rb') as f:
            latex_dict = pickle.load(f)
            for form in latex_dict:
                idf = 'FORMULA' + str(form) + '_'
                # we must \\dot in \rst doxygen
                # else there is a confusion with dot from graphviz.
                formula = latex_dict[form]["latex"].replace(r'\\', '\\\\')
                # escape \
                formula = latex_dict[form]["latex"].replace('\\', '\\\\')
                formula_type = latex_dict[form]["label"]  # inline or not
                #formula = ''.join(formula)
                for line in source_lines:
                    if formula_type == 'inline':
                        rst.append(line.replace(idf, formula))
                    else:
                        indent = len(line) - len(line.lstrip())
                        rst.append(
                            line.replace(idf, textwrap.indent(formula,
                                                              indent * ' ')))
                source_lines = list(rst)
                rst = []

    # Replace .py with new results.
    with open(target, 'w') as f:
        for line in source_lines:
            f.write(line)
    shutil.move(target, inoutfile)


