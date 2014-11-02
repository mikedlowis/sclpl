/**
  @file tree.c
  @brief See header for details
  $Revision$
  $HeadURL$
  */
#include "tree.h"

static void tree_free(void* p_obj) {
    tree_t* p_tree = ((tree_t*)p_obj);
    if (NULL != p_tree->ptr.tok) {
        mem_release(p_tree->ptr.tok);
    }
}

tree_t* tree_new(tree_tag_t tag, void* p_obj) {
    tree_t* p_tree = (tree_t*)mem_allocate(sizeof(tree_t), &tree_free);
    p_tree->tag     = tag;
    p_tree->ptr.tok = (lex_tok_t*)p_obj;
    return p_tree;
}

tree_t* tree_get_child(tree_t* p_tree, size_t idx) {
    tree_t* child = NULL;
    if (p_tree->tag == TREE) {
        vec_t* vec = p_tree->ptr.vec;
        if (idx < vec_size(vec))
            child = vec_at(vec, idx);
    }
    return child;
}

void* tree_get_val(tree_t* p_tree) {
    void* ret = NULL;
    if (p_tree->tag == ATOM) {
        ret = p_tree->ptr.tok->value;
    }
    return ret;
}

void* tree_get_child_val(tree_t* p_tree, size_t idx) {
    void* ret = NULL;
    tree_t* child = tree_get_child(p_tree,idx);
    if (child != NULL) {
        ret = tree_get_val(child);
    }
    return ret;
}

bool tree_is_formtype(tree_t* p_tree, const char* val) {
    bool ret = false;
    tree_t* child = tree_get_child(p_tree, 0);
    if ((NULL != child) && (child->tag == ATOM)) {
        lex_tok_t* token = child->ptr.tok;
        if ((token->type == T_ID) &&
            (0 == strcmp(val, (char*)token->value))) {
            ret = true;
        }
    }
    return ret;
}
