import { useState, useEffect } from 'react';
import { Prism as SyntaxHighlighter } from 'react-syntax-highlighter';
import { oneLight } from 'react-syntax-highlighter/dist/esm/styles/prism';
import ReactMarkdown from 'react-markdown';
import remarkGfm from 'remark-gfm';
import IkeaStyleHeader from './IkeaStyleHeader';
import IkeaStyleFooter from './IkeaStyleFooter';
import PaidContentLock from './PaidContentLock';
import CommentSection from './CommentSection';
import LikeButton from './LikeButton';
import FavoriteButton from './FavoriteButton';
import './IkeaBlogDetail.css';

// 代码块复制按钮组件
const CodeBlock = ({ children, className, ...props }) => {
  const [copied, setCopied] = useState(false);
  const match = /language-(\w+)/.exec(className || '');
  const language = match ? match[1] : '';

  const handleCopy = () => {
    navigator.clipboard.writeText(children);
    setCopied(true);
    setTimeout(() => setCopied(false), 2000);
  };

  return match ? (
    <div className="ikea-code-block">
      <div className="ikea-code-header">
        <span className="ikea-code-language">{language}</span>
        <button 
          className={`ikea-code-copy ${copied ? 'copied' : ''}`}
          onClick={handleCopy}
        >
          {copied ? (
            <>
              <svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2">
                <polyline points="20,6 9,17 4,12"></polyline>
              </svg>
              已复制
            </>
          ) : (
            <>
              <svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2">
                <rect x="9" y="9" width="13" height="13" rx="2" ry="2"></rect>
                <path d="M5 15H4a2 2 0 0 1-2-2V4a2 2 0 0 1 2-2h9a2 2 0 0 1 2 2v1"></path>
              </svg>
              复制
            </>
          )}
        </button>
      </div>
      <SyntaxHighlighter
        style={oneLight}
        language={language}
        PreTag="div"
        customStyle={{
          margin: 0,
          borderRadius: '0 0 8px 8px',
          fontSize: '14px',
          lineHeight: '1.5'
        }}
        {...props}
      >
        {String(children).replace(/\n$/, '')}
      </SyntaxHighlighter>
    </div>
  ) : (
    <code className="ikea-inline-code" {...props}>
      {children}
    </code>
  );
};

const IkeaBlogDetail = ({ id, onBack, onNavigate }) => {
  const [post, setPost] = useState(null);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState(null);
  const [relatedPosts, setRelatedPosts] = useState([]);
  const [favorited, setFavorited] = useState(false);
  const [likesCount, setLikesCount] = useState(0);
  const [shareSuccess, setShareSuccess] = useState(false);
  const [isUnlocked, setIsUnlocked] = useState(false);
  const [token, setToken] = useState(null);
  const [currentUser, setCurrentUser] = useState(null);

  useEffect(() => {
    // 从localStorage获取token和用户信息
    const userToken = localStorage.getItem('userToken');
    const userInfo = localStorage.getItem('userInfo');
    
    if (userToken) {
      setToken(userToken);
    }
    
    if (userInfo) {
      try {
        setCurrentUser(JSON.parse(userInfo));
      } catch (error) {
        console.error('解析用户信息失败:', error);
      }
    }

    if (id) {
      fetchPost();
      checkFavoriteStatus();
    }
  }, [id]);

  const fetchPost = async () => {
    try {
      setLoading(true);
      setError(null);
      
      const response = await fetch(`/api/posts/${id}`);
      if (response.ok) {
        const data = await response.json();
        setPost(data);
        setLikesCount(data.likes || 0);
        
        // 获取相关文章
        if (data.category || (data.tags && data.tags.length > 0)) {
          fetchRelatedPosts(data);
        }
      } else if (response.status === 404) {
        setError('文章不存在');
      } else {
        setError('获取文章失败');
      }
    } catch (error) {
      console.error('获取文章错误:', error);
      setError('网络错误，请稍后重试');
    } finally {
      setLoading(false);
    }
  };

  const fetchRelatedPosts = async (currentPost) => {
    try {
      const params = new URLSearchParams({
        status: 'published',
        limit: '6'
      });
      
      if (currentPost.category) {
        params.append('category', currentPost.category);
      }
      
      const response = await fetch(`/api/posts?${params.toString()}`);
      if (response.ok) {
        const data = await response.json();
        const related = data.posts
          .filter(p => p.id !== currentPost.id)
          .slice(0, 3);
        setRelatedPosts(related);
      }
    } catch (error) {
      console.error('获取相关文章错误:', error);
    }
  };

  const checkFavoriteStatus = async () => {
    try {
      const token = localStorage.getItem('userToken');
      if (!token) {
        setFavorited(false);
        return;
      }

      const response = await fetch(`/api/favorites/check/${id}`, {
        headers: {
          'Authorization': `Bearer ${token}`
        }
      });

      if (response.ok) {
        const data = await response.json();
        setFavorited(data.favorited);
      } else {
        setFavorited(false);
      }
    } catch (error) {
      console.error('检查收藏状态失败:', error);
      setFavorited(false);
    }
  };

  const handleFavorite = async () => {
    try {
      const token = localStorage.getItem('userToken');
      if (!token) {
        // 未登录，跳转到登录页面
        window.location.hash = '#account';
        return;
      }

      if (favorited) {
        // 取消收藏
        const response = await fetch(`/api/favorites/${id}`, {
          method: 'DELETE',
          headers: {
            'Authorization': `Bearer ${token}`
          }
        });

        if (response.ok) {
          setFavorited(false);
          // 触发自定义事件通知其他组件
          window.dispatchEvent(new CustomEvent('favoritesChanged'));
        } else {
          const data = await response.json();
          console.error('取消收藏失败:', data.error);
        }
      } else {
        // 添加收藏
        const response = await fetch('/api/favorites', {
          method: 'POST',
          headers: {
            'Content-Type': 'application/json',
            'Authorization': `Bearer ${token}`
          },
          body: JSON.stringify({ postId: parseInt(id) })
        });

        if (response.ok) {
          setFavorited(true);
          // 触发自定义事件通知其他组件
          window.dispatchEvent(new CustomEvent('favoritesChanged'));
        } else {
          const data = await response.json();
          console.error('收藏失败:', data.error);
        }
      }
    } catch (error) {
      console.error('收藏操作失败:', error);
    }
  };

  const handleLike = async () => {
    try {
      const response = await fetch(`/api/posts/${id}/like`, {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
      });
      if (response.ok) {
        const data = await response.json();
        setLikesCount(data.likes);
      }
    } catch (error) {
      console.error('点赞失败:', error);
    }
  };

  const handleShare = async () => {
    const url = `${window.location.origin}${window.location.pathname}#blog/${id}`;
    
    try {
      if (navigator.share) {
        await navigator.share({
          title: post?.title,
          text: post?.excerpt,
          url: url,
        });
      } else {
        await navigator.clipboard.writeText(url);
        setShareSuccess(true);
        setTimeout(() => setShareSuccess(false), 2000);
      }
    } catch (error) {
      console.error('分享失败:', error);
    }
  };

  const formatDate = (dateString) => {
    if (!dateString) return '未知日期';
    
    const date = new Date(dateString);
    
    // 检查日期是否有效
    if (isNaN(date.getTime())) {
      return '未知日期';
    }
    
    return date.toLocaleDateString('zh-CN', {
      year: 'numeric',
      month: 'long',
      day: 'numeric'
    });
  };

  const estimateReadingTime = (content) => {
    const wordsPerMinute = 200;
    const words = content.replace(/[^\w\s]/g, '').split(/\s+/).length;
    return Math.ceil(words / wordsPerMinute);
  };

  if (loading) {
    return (
      <div className="ikea-blog-detail">
        <IkeaStyleHeader currentPage="blog" onNavigate={onNavigate} />
        <main className="ikea-blog-main">
          <div className="ikea-container">
            <div className="ikea-blog-loading">
              <div className="ikea-spinner"></div>
              <p>加载中...</p>
            </div>
          </div>
        </main>
        <IkeaStyleFooter />
      </div>
    );
  }

  if (error || !post) {
    return (
      <div className="ikea-blog-detail">
        <IkeaStyleHeader currentPage="blog" onNavigate={onNavigate} />
        <main className="ikea-blog-main">
          <div className="ikea-container">
            <div className="ikea-blog-error">
              <svg width="64" height="64" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="1">
                <circle cx="12" cy="12" r="10"></circle>
                <line x1="12" y1="8" x2="12" y2="12"></line>
                <line x1="12" y1="16" x2="12.01" y2="16"></line>
              </svg>
              <h2>{error || '文章未找到'}</h2>
              <p>抱歉，您要查看的文章不存在或已被删除。</p>
              <button onClick={onBack} className="ikea-btn ikea-btn-primary">
                返回首页
              </button>
            </div>
          </div>
        </main>
        <IkeaStyleFooter />
      </div>
    );
  }

  return (
    <div className="ikea-blog-detail">
      <IkeaStyleHeader currentPage="blog" onNavigate={onNavigate} />
      
      <main className="ikea-blog-main">
        <div className="ikea-container">
          {/* 返回按钮 */}
          <div className="ikea-blog-nav">
            <button onClick={() => {
              // 返回首页并滚动到内容区域
              window.location.hash = '';
              setTimeout(() => {
                const contentSection = document.getElementById('content-section');
                if (contentSection) {
                  contentSection.scrollIntoView({ behavior: 'smooth' });
                }
              }, 100);
            }} className="ikea-blog-back">
              <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2">
                <path d="M19 12H5M12 19l-7-7 7-7"/>
              </svg>
              返回
            </button>
          </div>

          {/* 文章封面图 - 全宽无边界 */}
          {post.image && (
            <div className="ikea-blog-hero-image">
              <img src={post.image} alt={post.title} />
              <div className="ikea-blog-hero-overlay">
                <div className="ikea-container">
                  <div className="ikea-blog-hero-content">
                    <div className="ikea-blog-meta">
                      {post.category && (
                        <span className="ikea-blog-category">{post.category}</span>
                      )}
                      <span className="ikea-blog-date">{formatDate(post.date)}</span>
                      <span className="ikea-blog-reading-time">
                        <svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2">
                          <circle cx="12" cy="12" r="10"></circle>
                          <polyline points="12,6 12,12 16,14"></polyline>
                        </svg>
                        {estimateReadingTime(post.content || '')} 分钟阅读
                      </span>
                      {post.is_paid && post.price > 0 && (
                        <span className="ikea-blog-paid-badge">
                          <svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2">
                            <rect x="3" y="11" width="18" height="11" rx="2" ry="2"></rect>
                            <circle cx="12" cy="16" r="1"></circle>
                            <path d="M7 11V7a5 5 0 0 1 10 0v4"></path>
                          </svg>
                          付费内容 ¥{post.price}
                        </span>
                      )}
                    </div>
                    
                    <h1 className="ikea-blog-title">{post.title}</h1>
                    
                    {post.excerpt && (
                      <p className="ikea-blog-excerpt">{post.excerpt}</p>
                    )}

                    {/* 文章标签 */}
                    {post.tags && post.tags.length > 0 && (
                      <div className="ikea-blog-tags">
                        {post.tags.map((tag, index) => (
                          <span key={index} className="ikea-blog-tag">
                            {tag}
                          </span>
                        ))}
                      </div>
                    )}
                  </div>
                </div>
              </div>
            </div>
          )}

          {/* 无封面图时的头部 */}
          {!post.image && (
            <div className="ikea-blog-header-section">
              <div className="ikea-container">
                <div className="ikea-blog-header-content">
                  <div className="ikea-blog-meta">
                    {post.category && (
                      <span className="ikea-blog-category">{post.category}</span>
                    )}
                    <span className="ikea-blog-date">{formatDate(post.date)}</span>
                    <span className="ikea-blog-reading-time">
                      <svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2">
                        <circle cx="12" cy="12" r="10"></circle>
                        <polyline points="12,6 12,12 16,14"></polyline>
                      </svg>
                      {estimateReadingTime(post.content || '')} 分钟阅读
                    </span>
                    {post.is_paid && post.price > 0 && (
                      <span className="ikea-blog-paid-badge">
                        <svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2">
                          <rect x="3" y="11" width="18" height="11" rx="2" ry="2"></rect>
                          <circle cx="12" cy="16" r="1"></circle>
                          <path d="M7 11V7a5 5 0 0 1 10 0v4"></path>
                        </svg>
                        付费内容 ¥{post.price}
                      </span>
                    )}
                  </div>
                  
                  <h1 className="ikea-blog-title">{post.title}</h1>
                  
                  {post.excerpt && (
                    <p className="ikea-blog-excerpt">{post.excerpt}</p>
                  )}

                  {/* 文章标签 */}
                  {post.tags && post.tags.length > 0 && (
                    <div className="ikea-blog-tags">
                      {post.tags.map((tag, index) => (
                        <span key={index} className="ikea-blog-tag">
                          {tag}
                        </span>
                      ))}
                    </div>
                  )}
                </div>
              </div>
            </div>
          )}

          {/* 文章内容区域 */}
          <div className="ikea-blog-content-section">
            <div className="ikea-container">
              <div className="ikea-blog-content-wrapper">
                <article className="ikea-blog-content">
                  {post.is_paid && post.price > 0 && !isUnlocked ? (
                    <PaidContentLock 
                      post={post} 
                      onPaymentSuccess={() => setIsUnlocked(true)}
                    />
                  ) : (
                    <div className="ikea-blog-markdown">
                      <ReactMarkdown
                        remarkPlugins={[remarkGfm]}
                        components={{
                          code: CodeBlock,
                          h1: ({children}) => <h1 className="ikea-markdown-h1">{children}</h1>,
                          h2: ({children}) => <h2 className="ikea-markdown-h2">{children}</h2>,
                          h3: ({children}) => <h3 className="ikea-markdown-h3">{children}</h3>,
                          h4: ({children}) => <h4 className="ikea-markdown-h4">{children}</h4>,
                          p: ({children}) => <p className="ikea-markdown-p">{children}</p>,
                          ul: ({children}) => <ul className="ikea-markdown-ul">{children}</ul>,
                          ol: ({children}) => <ol className="ikea-markdown-ol">{children}</ol>,
                          li: ({children}) => <li className="ikea-markdown-li">{children}</li>,
                          blockquote: ({children}) => <blockquote className="ikea-markdown-blockquote">{children}</blockquote>,
                          table: ({children}) => <div className="ikea-table-wrapper"><table className="ikea-markdown-table">{children}</table></div>,
                          a: ({href, children}) => <a href={href} className="ikea-markdown-link" target="_blank" rel="noopener noreferrer">{children}</a>,
                          img: ({src, alt}) => <img src={src} alt={alt} className="ikea-markdown-img" />
                        }}
                      >
                        {post.content}
                      </ReactMarkdown>
                    </div>
                  )}
                </article>

                {/* 文章互动区 */}
                <footer className="ikea-blog-footer">
                  <div className="ikea-blog-actions">
                    <div className="ikea-blog-action-wrapper">
                      <FavoriteButton 
                        postId={id} 
                        token={token} 
                        currentUser={currentUser}
                        onFavoriteChange={(isFavorited) => {
                          setFavorited(isFavorited);
                        }}
                      />
                    </div>

                    <div className="ikea-blog-action-wrapper">
                      <LikeButton 
                        postId={id} 
                        token={token} 
                        currentUser={currentUser}
                        onLikeChange={(count, liked) => {
                          setLikesCount(count);
                        }}
                      />
                    </div>

                    <button 
                      className={`ikea-blog-action ${shareSuccess ? 'success' : ''}`}
                      onClick={handleShare}
                    >
                      <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2">
                        <circle cx="18" cy="5" r="3"></circle>
                        <circle cx="6" cy="12" r="3"></circle>
                        <circle cx="18" cy="19" r="3"></circle>
                        <line x1="8.59" y1="13.51" x2="15.42" y2="17.49"></line>
                        <line x1="15.41" y1="6.51" x2="8.59" y2="10.49"></line>
                      </svg>
                      {shareSuccess ? '已复制链接' : '分享'}
                    </button>
                  </div>

                  <div className="ikea-blog-stats">
                    <span className="ikea-blog-stat">
                      <svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2">
                        <path d="M1 12s4-8 11-8 11 8 11 8-4 8-11 8-11-8-11-8z"></path>
                        <circle cx="12" cy="12" r="3"></circle>
                      </svg>
                      阅读量统计功能开发中
                    </span>
                  </div>
                </footer>

                {/* 评论区 */}
                <CommentSection 
                  postId={parseInt(id)} 
                  token={token} 
                  currentUser={currentUser}
                  theme="ikea"
                />
              </div>
            </div>
          </div>

          {/* 相关文章 */}
          {relatedPosts.length > 0 && (
            <section className="ikea-blog-related">
              <h3 className="ikea-blog-related-title">相关文章</h3>
              <div className="ikea-blog-related-grid">
                {relatedPosts.map(relatedPost => (
                  <article 
                    key={relatedPost.id} 
                    className="ikea-blog-related-card"
                    onClick={() => {
                      window.location.hash = `#blog/${relatedPost.id}`;
                    }}
                  >
                    {relatedPost.image && (
                      <div className="ikea-blog-related-image">
                        <img src={relatedPost.image} alt={relatedPost.title} />
                      </div>
                    )}
                    <div className="ikea-blog-related-content">
                      <h4 className="ikea-blog-related-card-title">{relatedPost.title}</h4>
                      {relatedPost.excerpt && (
                        <p className="ikea-blog-related-excerpt">{relatedPost.excerpt}</p>
                      )}
                      <div className="ikea-blog-related-meta">
                        <span className="ikea-blog-related-date">
                          {formatDate(relatedPost.date)}
                        </span>
                        {relatedPost.category && (
                          <span className="ikea-blog-related-category">
                            {relatedPost.category}
                          </span>
                        )}
                      </div>
                    </div>
                  </article>
                ))}
              </div>
            </section>
          )}
        </div>
      </main>

      <IkeaStyleFooter />
    </div>
  );
};

export default IkeaBlogDetail;