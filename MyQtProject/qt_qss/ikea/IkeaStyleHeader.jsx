import { useState, useEffect } from 'react';
import BingWallpaperSelector from './BingWallpaperSelector';
import './IkeaStyleHeader.css';

const IkeaStyleHeader = ({ currentPage, onNavigate }) => {
  const [isMenuOpen, setIsMenuOpen] = useState(false);
  const [isScrolled, setIsScrolled] = useState(false);
  const [user, setUser] = useState(null);

  useEffect(() => {
    const handleScroll = () => {
      setIsScrolled(window.scrollY > 50);
    };

    window.addEventListener('scroll', handleScroll);
    return () => window.removeEventListener('scroll', handleScroll);
  }, []);

  // 检查用户登录状态
  useEffect(() => {
    const checkUserStatus = () => {
      const token = localStorage.getItem('userToken');
      const userInfo = localStorage.getItem('userInfo');
      if (token && userInfo) {
        try {
          setUser(JSON.parse(userInfo));
        } catch (e) {
          setUser(null);
        }
      } else {
        setUser(null);
      }
    };

    checkUserStatus();

    // 监听用户登录/登出事件
    window.addEventListener('userLoggedIn', checkUserStatus);
    window.addEventListener('userLoggedOut', checkUserStatus);
    window.addEventListener('userUpdated', checkUserStatus);
    window.addEventListener('storage', checkUserStatus);

    return () => {
      window.removeEventListener('userLoggedIn', checkUserStatus);
      window.removeEventListener('userLoggedOut', checkUserStatus);
      window.removeEventListener('userUpdated', checkUserStatus);
      window.removeEventListener('storage', checkUserStatus);
    };
  }, []);

  const handleNavClick = (page) => {
    setIsMenuOpen(false);
    if (onNavigate) {
      onNavigate(page);
    }
  };

  const toggleMenu = () => {
    setIsMenuOpen(!isMenuOpen);
  };

  return (
    <>
      {/* 顶部信息栏 */}
      <div className="ikea-top-header">
        <div className="ikea-top-header-content">
          <div className="ikea-top-header-left">
            <a href="#" className="ikea-top-header-link">免费阅读</a>
            <a href="#" className="ikea-top-header-link">技术分享</a>
            <a href="#" className="ikea-top-header-link">开源项目</a>
          </div>
          <div className="ikea-top-header-right">
            {user ? (
              <>
                <span className="ikea-top-header-welcome">欢迎, {user.username}</span>
                <span>|</span>
                <a href="#" className="ikea-top-header-link" onClick={(e) => {
                  e.preventDefault();
                  localStorage.removeItem('userToken');
                  localStorage.removeItem('userInfo');
                  setUser(null);
                  window.dispatchEvent(new CustomEvent('userLoggedOut'));
                }}>退出</a>
              </>
            ) : (
              <>
                <a href="#account" className="ikea-top-header-link" onClick={(e) => {
                  e.preventDefault();
                  handleNavClick('account');
                }}>登录</a>
                <span>|</span>
                <a href="#account" className="ikea-top-header-link" onClick={(e) => {
                  e.preventDefault();
                  handleNavClick('account');
                }}>注册</a>
              </>
            )}
          </div>
        </div>
      </div>

      {/* 主导航 */}
      <header className={`ikea-main-header ${isScrolled ? 'scrolled' : ''}`}>
        <nav className="ikea-main-nav">
          <a href="#" className="ikea-logo" onClick={(e) => {
            e.preventDefault();
            handleNavClick('home');
          }}>
            <img src="/img/logo_simple_white.png" alt="Shigure Logo" className="ikea-logo-img" />
            <span className="ikea-logo-text">Shigure</span>
          </a>
          
          <ul className={`ikea-nav-menu ${isMenuOpen ? 'active' : ''}`}>
            <li className="ikea-nav-item">
              <a 
                href="#" 
                className={`ikea-nav-link ${currentPage === 'home' ? 'active' : ''}`}
                onClick={(e) => {
                  e.preventDefault();
                  handleNavClick('home');
                }}
              >
                首页
              </a>
            </li>

            <li className="ikea-nav-item">
              <a 
                href="#about" 
                className={`ikea-nav-link ${currentPage === 'about' ? 'active' : ''}`}
                onClick={(e) => {
                  e.preventDefault();
                  handleNavClick('about');
                }}
              >
                关于我
              </a>
            </li>
            <li className="ikea-nav-item">
              <a 
                href="#contact" 
                className={`ikea-nav-link ${currentPage === 'contact' ? 'active' : ''}`}
                onClick={(e) => {
                  e.preventDefault();
                  handleNavClick('contact');
                }}
              >
                联系
              </a>
            </li>
          </ul>
          
          <div className="ikea-nav-actions">
            <BingWallpaperSelector />
            
            <button 
              className="ikea-icon-btn ikea-tooltip" 
              data-tooltip="收藏夹"
              onClick={() => handleNavClick('favorites')}
            >
              <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2">
                <path d="M19 21l-7-5-7 5V5a2 2 0 0 1 2-2h10a2 2 0 0 1 2 2z"></path>
              </svg>
            </button>
            
            <button 
              className="ikea-icon-btn ikea-tooltip ikea-user-avatar-btn" 
              data-tooltip={user ? "个人主页" : "用户中心"}
              onClick={() => handleNavClick(user ? 'profile' : 'account')}
            >
              {user && user.avatar ? (
                <img 
                  src={user.avatar} 
                  alt="用户头像" 
                  className="ikea-user-avatar"
                  onError={(e) => {
                    e.target.style.display = 'none';
                    e.target.nextElementSibling.style.display = 'block';
                  }}
                />
              ) : null}
              <svg 
                width="20" 
                height="20" 
                viewBox="0 0 24 24" 
                fill="none" 
                stroke="currentColor" 
                strokeWidth="2"
                style={{ display: user && user.avatar ? 'none' : 'block' }}
              >
                <path d="M20 21v-2a4 4 0 0 0-4-4H8a4 4 0 0 0-4 4v2"></path>
                <circle cx="12" cy="7" r="4"></circle>
              </svg>
            </button>
            
            <button className="ikea-menu-toggle" onClick={toggleMenu}>
              <svg width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2">
                <line x1="3" y1="6" x2="21" y2="6"></line>
                <line x1="3" y1="12" x2="21" y2="12"></line>
                <line x1="3" y1="18" x2="21" y2="18"></line>
              </svg>
            </button>
          </div>
        </nav>
      </header>

      {/* 面包屑导航 */}
      {currentPage !== 'home' && (
        <div className="ikea-breadcrumb">
          <div className="ikea-breadcrumb-content">
            <ul className="ikea-breadcrumb-list">
              <li className="ikea-breadcrumb-item">
                <a 
                  href="#" 
                  className="ikea-breadcrumb-link"
                  onClick={(e) => {
                    e.preventDefault();
                    handleNavClick('home');
                  }}
                >
                  首页
                </a>
              </li>
              <li className="ikea-breadcrumb-separator">/</li>
              <li className="ikea-breadcrumb-item">
                {currentPage === 'about' && '关于我'}
                {currentPage === 'contact' && '联系我'}
                {currentPage === 'blog' && '博客详情'}
                {currentPage === 'account' && '用户中心'}
                {currentPage === 'admin' && '管理后台'}
                {currentPage === 'favorites' && '我的收藏'}
                {currentPage === 'profile' && '个人主页'}
              </li>
            </ul>
          </div>
        </div>
      )}
    </>
  );
};

export default IkeaStyleHeader;