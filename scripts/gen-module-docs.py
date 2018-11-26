#!/usr/bin/env python3

import sys
import os
import re
import glob

import yaml

def main():
    if len(sys.argv) != 3:
        sys.exit("USAGE: get-module-docs.py <path to docs file> <path to workspace>")

    docs_path = sys.argv[1]
    workspace_dir = sys.argv[2]

    process_shaders(workspace_dir, docs_path)
    process_joysticks(workspace_dir, docs_path)

def process_joysticks(workspace_dir, docs_path):
    docs = []
    for path in glob.glob(os.path.join(workspace_dir, "*", "*.joystick.yml")):
        base = os.path.basename(path)
        name = os.path.splitext(os.path.splitext(base)[0])[0]
        joysticks_dir = os.path.normpath(path).split(os.sep)[-2]
        if joysticks_dir == "personal":
            continue

        doc = read_joystick(path)
        doc["type"] = "{}/{}".format(joysticks_dir, name)

        docs.append(doc)

    md = "---\n"
    md += "title: Joystick Modules\n"
    md += "permalink: /docs/joystick_modules/\n"
    md += "---\n\n"
    md += generate_joystick_markdown(docs)

    with open(os.path.join(docs_path, "joystick_modules.md"), "w") as f:
        f.write(md)

def process_shaders(workspace_dir, docs_path):
    docs = []
    for path in glob.glob(os.path.join(workspace_dir, "*", "*.frag")):
        base = os.path.basename(path)
        name = os.path.splitext(base)[0]
        shaders_dir = os.path.normpath(path).split(os.sep)[-2]
        if shaders_dir == "personal":
            continue

        doc = read_shader(path)
        doc["type"] = "{}/{}".format(shaders_dir, name)

        docs.append(doc)

    md = "---\n"
    md += "title: Shader Modules\n"
    md += "permalink: /docs/shader_modules/\n"
    md += "---\n\n"
    md += generate_shader_markdown(docs)

    with open(os.path.join(docs_path, "shader_modules.md"), "w") as f:
        f.write(md)

def read_joystick(path):
    with open(path, 'r') as f:
        y = yaml.load(f)

    outputs = []
    for k, v in y["axes"].items():
        outputs.append(v)

    for k, v in y["buttons"].items():
        outputs.append(v)

    return {"outputs": outputs, "device_substring": y["device"] , "name": y["name"]}

def generate_joystick_markdown(docs):
    md = ""
    md += "## Summary\n\n"
    md += "|Type|Name|Search String|\n"
    md += "|----|----|-------------|\n"
    for doc in docs:
        t = "[{}](#{})".format(doc["type"], "-".join(doc["device_substring"].split(" ")).lower())
        md += "|{}|{}|{}|\n".format(t, doc["name"], doc["device_substring"])

    md += "\n## Details\n\n"
    for doc in docs:
        md += "\n### {}\n\n".format(doc["name"])
        md += "Type: {}\n\n".format(doc["type"])
        md += "Search String: {}\n\n".format(doc["device_substring"])

        md += "\nOutputs:\n"
        for o in doc["outputs"]:
            md += "* *{}*\n".format(o)

    return md

def generate_shader_markdown(docs):
    md = ""
    md += "## Summary\n\n"
    md += "|Type|Name|Description|Author|\n"
    md += "|----|----|-----------|------|\n"
    for doc in docs:
        t = "[{}](#{})".format(doc["type"], "-".join(doc["name"].split(" ")).lower())
        md += "|{}|{}|{}|{}|\n".format(t, doc["name"], doc["desc"], doc["author"])

    md += "\n## Details\n\n"
    for doc in docs:
        md += "\n### {}\n\n".format(doc["name"])
        md += doc["desc"] + "\n\n"
        md += "Type: {}\n\n".format(doc["type"])
        md += "Inputs:\n"
        for i in doc["inputs"]:
            md += "* *{}* - {}\n".format(i["name"], i["desc"])

        md += "\nOutputs:\n"
        for o in doc["outputs"]:
            md += "* *{}* - {}\n".format(o["name"], o["desc"])

    return md

def read_shader(path):
    doc = {"inputs": [], "outputs": []}
    with open(path) as f:
        for line in f:
            m = re.match("DEFINE_INPUT\((\w+),", line)
            if m is not None:
                doc["inputs"].append({"name": m.group(1), "desc": read_desc(line)})
                continue

            m = re.match("DEFINE_OUTPUT_\d+\((\w+),", line)
            if m is not None:
                doc["outputs"].append({"name": m.group(1), "desc": read_desc(line)})
                continue

            m = re.match("// @name (.*)", line)
            if m is not None:
                doc["name"] = m.group(1)

            m = re.match("// @desc (.*)", line)
            if m is not None:
                doc["desc"] = m.group(1)

            m = re.match("// @author (.*)", line)
            if m is not None:
                doc["author"] = m.group(1)

    return doc

def read_desc(line):
    m = re.match(".*DESC\([\"'](.*?)[\"']\).*", line)
    if m is None:
        sys.exit("ERROR: Missing DESC on same line as DEFINE_INPUT or DEFINE_OUTPUT")

    return m.group(1)

# https://stackoverflow.com/questions/1175208/elegant-python-function-to-convert-camelcase-to-snake-case
def underscore(name):
    s1 = re.sub('(.)([A-Z][a-z]+)', r'\1_\2', name)
    return re.sub('([a-z0-9])([A-Z])', r'\1_\2', s1).lower()

if __name__ == "__main__":
    main()
