import os 
import sys
import zlib
import re 
import collections

class CommitNode:
    def __init__(self, commit_hash):
        """
        :type commit_hash: str
        """
        self.commit_hash = commit_hash
        self.parents = set()
        self.children = set()

def find_git_directory():
    current = os.getcwd()
    parent = os.path.dirname(current)
    while current != '/':
        files = os.listdir(current)
        if '.git' in files:
            return os.path.join(current,'.git')
        current = parent 
        parent = os.path.dirname(current)
    sys.stderr.write('Not inside a Git repository')
    exit(1)

def get_branches(dir):
    branch_heads = collections.OrderedDict()
    current = os.path.join(dir, os.path.join('refs', 'heads'))
    for branch in os.listdir(current):
        with open(os.path.join(current, branch), 'r') as branch_file:
            head = branch_file.read().strip()
            if head in branch_heads:
                branch_heads[head].append(branch)
            else:
                branch_heads[head] = [branch]
    return branch_heads

def find_parent_hashes(content):
    parents = []
    for match in re.findall("parent .{40}", content):
        parents.append(match[7:].strip())
    return parents
    
def generate_commit_graph(branch_heads, dir):
    nodes = collections.OrderedDict()
    objects_dir = os.path.join(dir, 'objects')
    for head_hash in branch_heads:
        if head_hash not in nodes:
            head_node = CommitNode(head_hash)
            stack = [head_node]
            nodes[head_hash] = head_node
        while stack:
            current_node = stack.pop()
            current_hash = current_node.commit_hash
            commit_path = os.path.join(os.path.join(objects_dir, current_hash[:2]), current_hash[2:])
            with open(commit_path, 'rb') as file:
                content = zlib.decompress(file.read()).decode()
                for parent_hash in find_parent_hashes(content):
                    if parent_hash not in nodes:
                        new_node = CommitNode(parent_hash)
                        new_node.children.add(current_hash)
                        current_node.parents.add(parent_hash)
                        nodes[parent_hash] = new_node
                        stack.append(new_node)
                    else:
                        nodes[parent_hash].children.add(current_hash)
                        current_node.parents.add(parent_hash)
    return nodes

def get_topo_order(nodes):
    order = []
    roots = []
    seen = set()
    for commit_hash,node in nodes.items():
        if not(node.parents):
            roots.append(commit_hash)

    stack = [roots[0]]

    for root in roots:
        if root not in seen:
            stack = [root]
        while stack:
            curr = stack.pop()
            if curr in seen:
                continue
            if len(nodes[curr].parents) > 1:
                new_path_stack = []
                queue = collections.deque()
                for parent in sorted(nodes[curr].parents):
                    if parent not in seen:
                        new_path_stack = [parent]
                        seen.add(parent)
                        while new_path_stack:
                            new_curr = new_path_stack.pop()
                            for parent in sorted(nodes[new_curr].parents):
                                if parent not in seen:
                                    new_path_stack.append(parent)
                            queue.append(new_curr)
                            seen.add(new_curr)
                while queue:
                    order.append(queue.pop())
            for child in sorted(nodes[curr].children):
                if child not in seen:
                    stack.append(child)
            order.append(curr)
            seen.add(curr)
    return order[::-1]

def topo_order_commits():
    git_dir = find_git_directory()
    branch_heads = get_branches(git_dir)
    heads = set()

    nodes = generate_commit_graph(branch_heads, git_dir)
    # for commit_hash, node in nodes.items():
    #     print(f'current: {commit_hash}')
    #     print(f'children: {node.children}')
    #     print(f'parents: {node.parents}')
    #     print('\n')
    topo_order = get_topo_order(nodes)

    length = len(topo_order)
    for i in range(length-1):
        curr_id = topo_order[i]
        curr_node = nodes[curr_id]
        next_id = topo_order[i+1]
        next_node = nodes[next_id]
        branch_string = ""
        if curr_id in branch_heads:
            branch_string = " " + " ".join(branch_heads[curr_id])
        if curr_id in next_node.children:
            print(curr_id + branch_string)
        else:
            print(curr_id)
            print('\n'.join(sorted(curr_node.parents)) + branch_string + '=' + '\n' + '\n' + '=')
        next = curr_id
    print(topo_order[length-1])


if __name__ == '__main__':
    topo_order_commits()

